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

#ifndef __LIBARCH_INTEL_PAGETABLE_H
#define __LIBARCH_INTEL_PAGETABLE_H

#include <Types.h>
#include <Memory.h>
#include <MemoryContext.h>

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libarch
 * @{
 *
 * @addtogroup libarch_intel
 * @{
 */

/**
 * Intel second level page table implementation.
 */
class IntelPageTable
{
  public:

    /**
     * Map a virtual address to a physical address.
     *
     * @param virt Virtual address.
     * @param phys Physical address.
     * @param access Memory access flags.
     *
     * @return Result code
     */
    MemoryContext::Result map(Address virt,
                              Address phys,
                              Memory::Access access);

    /**
     * Remove virtual address mapping.
     *
     * @param virt Virtual address.
     *
     * @return Result code
     */
    MemoryContext::Result unmap(Address virt);

    /**
     * Translate virtual address to physical address.
     *
     * @param virt Virtual address to lookup
     * @param phys On output contains the translated physical memory address
     *
     * @return Result code
     */
    MemoryContext::Result translate(Address virt, Address *phys) const;

    /**
     * Get Access flags for a virtual address.
     *
     * @param virt Virtual address to get Access flags for.
     * @param access MemoryAccess object pointer.
     *
     * @return Result code.
     */
    MemoryContext::Result access(Address virt, Memory::Access *access) const;

  private:

    /**
     * Convert MemoryAccess to page table flags.
     *
     * @param access Memory access flags to convert
     *
     * @return Page table flags
     */
    u32 flags(Memory::Access access) const;

  private:

    /** Array of page table entries. */
    u32 m_pages[1024];
};

/**
 * @}
 * @}
 * @}
 */

#endif /* __LIBARCH_INTEL_PAGETABLE_H */
