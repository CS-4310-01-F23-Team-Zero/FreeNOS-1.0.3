/*
 * Copyright (C) 2015 Niek Linnenbank
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <FreeNOS/System.h>
#include <SplitAllocator.h>
#include <MemoryBlock.h>
#include "ARMCore.h"
#include "ARMConstant.h"
#include "ARMFirstTable.h"

/**
 * @name First Level Entry Types.
 *
 * @see ARM Architecture Reference Manual, page 731.
 *
 * @{
 */
#define PAGE1_NONE       0
#define PAGE1_TABLE     (1 << 0)
#define PAGE1_SECTION   (1 << 1)

/**
 * @}
 */

/**
 * @name First Level Memory Types
 *
 * Inner cache: local L1 cache for one core.
 * Outer cache: shared L2 cache for a subset of multiple cores.
 * Write-Back cache: sync cache data later, let host continue immediately (faster).
 * Write-Through cache: sync cache and RAM simulatously (slower).
 * Allocate-On-Write: Each write claims a cache entry.
 * No-Allocate-On-Write: Write do no claim a cache entry, only write the RAM directly.
 *
 * @see https://en.wikipedia.org/wiki/Cache_(computing)
 * @{
 */

/** Disable all caching */
#define PAGE1_UNCACHED            (PAGE1_TEX)

/** Outer and Inner Write-Back. Allocate on write. */
#define PAGE1_CACHE_WRITEBACK     (PAGE1_TEX | PAGE1_CACHE | PAGE1_BUFFER)

/** Outer and Inner Write-Through. No allocate on write. */
#define PAGE1_CACHE_WRITETHROUGH  (PAGE1_CACHE)

/** Memory Mapped Device (Private) */
#define PAGE1_DEVICE_PRIV         ((1 << 13))

/** Memory Mapped Device (Shared) */
#define PAGE1_DEVICE_SHARED       (PAGE1_BUFFER)

#define PAGE1_TEX       (1 << 12)
#define PAGE1_CACHE     (1 << 3)
#define PAGE1_BUFFER    (1 << 2)
#define PAGE1_SHARED    (1 << 16)

/**
 * @}
 */

/**
 * @name Access permission flags
 * @{
 */

/** No-execution flag */
#define PAGE1_NOEXEC    (1 << 4)

/* Read-only flag */
#ifdef ARMV7
#define PAGE1_APX       (1 << 15)
#else
#define PAGE1_APX       (1 << 9)
#endif

/* User access permissions flag */
#define PAGE1_AP_USER   (1 << 11)

/* System access permissions flag */
#define PAGE1_AP_SYS    (1 << 10)

/**
 * @}
 */

/**
 * Entry inside the page directory of a given virtual address.
 *
 * @param vaddr Virtual Address.
 *
 * @return Index of the corresponding page directory entry.
 */
#define DIRENTRY(vaddr) \
    ((vaddr) >> DIRSHIFT)

ARMSecondTable * ARMFirstTable::getSecondTable(Address virt, SplitAllocator *alloc) const
{
    u32 entry = m_tables[ DIRENTRY(virt) ];

    // Check if the page table is present.
    if (!(entry & PAGE1_TABLE))
        return ZERO;
    else
        return (ARMSecondTable *) alloc->toVirtual(entry & PAGEMASK);
}

MemoryContext::Result ARMFirstTable::map(Address virt,
                                         Address phys,
                                         Memory::Access access,
                                         SplitAllocator *alloc)
{
    ARMSecondTable *table = getSecondTable(virt, alloc);
    Arch::Cache cache;
    Allocator::Range allocPhys, allocVirt;

    // Check if the page table is present.
    if (!table)
    {
        // Reject if already mapped as a (super)section
        if (m_tables[ DIRENTRY(virt) ] & PAGE1_SECTION)
            return MemoryContext::AlreadyExists;

        // Allocate a new page table
        allocPhys.address = 0;
        allocPhys.size = sizeof(ARMSecondTable);
        allocPhys.alignment = PAGESIZE;

        if (alloc->allocate(allocPhys, allocVirt) != Allocator::Success)
            return MemoryContext::OutOfMemory;

        MemoryBlock::set((void *)allocVirt.address, 0, PAGESIZE);

        // Assign to the page directory. Do not assign permission flags (only for direct sections).
        m_tables[ DIRENTRY(virt) ] = allocPhys.address | PAGE1_TABLE;
        cache.cleanData(&m_tables[DIRENTRY(virt)]);
        table = getSecondTable(virt, alloc);
    }
    return table->map(virt, phys, access);
}

MemoryContext::Result ARMFirstTable::mapLarge(Memory::Range range,
                                              SplitAllocator *alloc)
{
    Arch::Cache cache;

    if (range.size & 0xfffff)
        return MemoryContext::InvalidSize;

    if ((range.phys & ~PAGEMASK) || (range.virt & ~PAGEMASK))
        return MemoryContext::InvalidAddress;

    for (Size i = 0; i < range.size; i += MegaByte(1))
    {
        if (m_tables[ DIRENTRY(range.virt + i) ] & (PAGE1_TABLE | PAGE1_SECTION))
            return MemoryContext::AlreadyExists;

        m_tables[ DIRENTRY(range.virt + i) ] = (range.phys + i) | PAGE1_SECTION | flags(range.access);
        cache.cleanData(&m_tables[DIRENTRY(range.virt + i)]);
    }
    return MemoryContext::Success;
}

MemoryContext::Result ARMFirstTable::unmap(Address virt, SplitAllocator *alloc)
{
    ARMSecondTable *table = getSecondTable(virt, alloc);
    Arch::Cache cache;

    if (!table)
    {
        if (m_tables[DIRENTRY(virt)] & PAGE1_SECTION)
        {
            m_tables[DIRENTRY(virt)] = PAGE1_NONE;
            cache.cleanData(&m_tables[DIRENTRY(virt)]);
            return MemoryContext::Success;
        }
        else
            return MemoryContext::InvalidAddress;
    }
    else
        return table->unmap(virt);
}

MemoryContext::Result ARMFirstTable::translate(Address virt,
                                               Address *phys,
                                               SplitAllocator *alloc) const
{
    ARMSecondTable *table = getSecondTable(virt, alloc);
    if (!table)
    {
        if (m_tables[DIRENTRY(virt)] & PAGE1_SECTION)
        {
            const Address offsetInSection = virt % MegaByte(1);

            *phys = (m_tables[DIRENTRY(virt)] & SECTIONMASK) +
                    ((offsetInSection / PAGESIZE) * PAGESIZE);
            return MemoryContext::Success;
        }
        return MemoryContext::InvalidAddress;
    }
    else
        return table->translate(virt, phys);
}

MemoryContext::Result ARMFirstTable::access(Address virt,
                                            Memory::Access *access,
                                            SplitAllocator *alloc) const
{
    ARMSecondTable *table = getSecondTable(virt, alloc);
    if (!table)
        return MemoryContext::InvalidAddress;
    else
        return table->access(virt, access);
}

u32 ARMFirstTable::flags(Memory::Access access) const
{
    u32 f = PAGE1_AP_SYS;

    // Permissions
    if (!(access & Memory::Executable)) f |= PAGE1_NOEXEC;
    if ((access & Memory::User))        f |= PAGE1_AP_USER;
    if (!(access & Memory::Writable))   f |= PAGE1_APX;

    // Caching
    if (access & Memory::Device)        f |= PAGE1_DEVICE_SHARED;
    else if (access & Memory::Uncached) f |= PAGE1_UNCACHED;
    else                                f |= PAGE1_CACHE_WRITEBACK;

    return f;
}

inline void ARMFirstTable::releasePhysical(SplitAllocator *alloc,
                                           const Address phys)
{
    // Some pages that are part of the boot core's memory region
    // are mapped on secondary cores. They can't be released there.
    const Address allocBase = alloc->base();
    const Size allocSize = alloc->size();
    if (phys < allocBase || phys > allocBase + allocSize)
    {
        return;
    }

    // Note that some pages may have double mappings.
    // Avoid attempting to release the same page twice or more.
    if (alloc->isAllocated(phys))
    {
        alloc->release(phys);
    }
}

MemoryContext::Result ARMFirstTable::releaseRange(const Memory::Range range,
                                                  SplitAllocator *alloc)
{
    Address phys;

    // Walk the full range of memory specified
    for (Size addr = range.virt; addr < range.virt + range.size; addr += PAGESIZE)
    {
        ARMSecondTable *table = getSecondTable(addr, alloc);
        if (table == ZERO)
        {
            return MemoryContext::InvalidAddress;
        }

        if (table->translate(addr, &phys) != MemoryContext::Success)
        {
            return MemoryContext::InvalidAddress;
        }

        releasePhysical(alloc, phys);
        table->unmap(addr);
    }

    return MemoryContext::Success;
}

MemoryContext::Result ARMFirstTable::releaseSection(const Memory::Range range,
                                                    SplitAllocator *alloc,
                                                    const bool tablesOnly)
{
    Address phys;

    // Input must be aligned to section address
    if (range.virt & ~SECTIONMASK)
    {
        return MemoryContext::InvalidAddress;
    }

    // Walk the page directory
    for (Size addr = range.virt; addr < range.virt + range.size; addr += MegaByte(1))
    {
        ARMSecondTable *table = getSecondTable(addr, alloc);
        if (!table)
        {
            continue;
        }

        // Release mapped pages, if requested
        if (!tablesOnly)
        {
            for (Size i = 0; i < MegaByte(1); i += PAGESIZE)
            {
                if (table->translate(i, &phys) == MemoryContext::Success)
                {
                    releasePhysical(alloc, phys);
                }
            }
        }
        // Release page table
        alloc->release(m_tables[ DIRENTRY(addr) ] & PAGEMASK);
        m_tables[ DIRENTRY(addr) ] = 0;
    }

    return MemoryContext::Success;
}
