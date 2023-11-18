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

#include <ByteOrder.h>
#include <MemoryBlock.h>
#include "NetworkServer.h"
#include "NetworkDevice.h"
#include "Ethernet.h"
#include "EthernetAddress.h"
#include "ICMP.h"

Ethernet::Ethernet(NetworkServer &server,
                   NetworkDevice &device)
    : NetworkProtocol(server, device, *this)
{
    m_arp  = ZERO;
    m_ipv4 = ZERO;
    MemoryBlock::set(&m_address, 0, sizeof(m_address));
}

Ethernet::~Ethernet()
{
}

FileSystem::Result Ethernet::initialize()
{
    m_device.getAddress(&m_address);

    m_server.registerDirectory(this, "/ethernet");
    m_server.registerFile(new EthernetAddress(m_server.getNextInode(), this), "/ethernet/address");

    return FileSystem::Success;
}

FileSystem::Result Ethernet::getAddress(Ethernet::Address *address)
{
    MemoryBlock::copy(address, &m_address, sizeof(Ethernet::Address));
    return FileSystem::Success;
}

FileSystem::Result Ethernet::setAddress(const Ethernet::Address *address)
{
    const FileSystem::Result result = m_device.setAddress(address);
    if (result == FileSystem::Success)
    {
        MemoryBlock::copy(&m_address, address, sizeof(Ethernet::Address));
    }

    return result;
}

void Ethernet::setARP(::ARP *arp)
{
    m_arp = arp;
}

void Ethernet::setIP(::IPV4 *ip)
{
    m_ipv4 = ip;
}

const String Ethernet::toString(const Ethernet::Address address)
{
    String s;

    s << Number::Hex;
    s << address.addr[0] << ":"
      << address.addr[1] << ":"
      << address.addr[2] << ":"
      << address.addr[3] << ":"
      << address.addr[4] << ":"
      << address.addr[5];

    return s;
}

FileSystem::Result Ethernet::getTransmitPacket(NetworkQueue::Packet **pkt,
                                               const void *address,
                                               const Size addressSize,
                                               const NetworkProtocol::Identifier protocol,
                                               const Size payloadSize)
{
    *pkt = m_device.getTransmitQueue()->get();
    if (!*pkt)
    {
        return FileSystem::RetryAgain;
    }

    Ethernet::Header *ether = (Ethernet::Header *) ((*pkt)->data + (*pkt)->size);
    MemoryBlock::copy(&ether->source, &m_address, sizeof(Address));
    MemoryBlock::copy(&ether->destination, address, sizeof(Address));

    switch (protocol)
    {
        case NetworkProtocol::IPV4:
            writeBe16(&ether->type, IPV4);
            break;

        case NetworkProtocol::ARP:
            writeBe16(&ether->type, ARP);
            break;

        default:
            ERROR("unsupported protocol: " << (int) protocol);
            return FileSystem::NotSupported;
    }

    (*pkt)->size += sizeof(Ethernet::Header);
    return FileSystem::Success;
}

FileSystem::Result Ethernet::process(const NetworkQueue::Packet *pkt,
                                     const Size offset)
{
    const Ethernet::Header *ether = (const Ethernet::Header *) (pkt->data + offset);
    const u16 type = readBe16(&ether->type);

    DEBUG("packet: size = " << pkt->size << " payload = " << *pkt);

    switch (type)
    {
        case Ethernet::ARP:
            if (m_arp)
                return m_arp->process(pkt, offset + sizeof(Ethernet::Header));
            break;

        case Ethernet::IPV4:
            if (m_ipv4)
                return m_ipv4->process(pkt, offset + sizeof(Ethernet::Header));
            break;

        default:
            DEBUG("dropped unknown ethernet type: " << (int) type);
            break;
    }

    return FileSystem::InvalidArgument;
}

Log & operator << (Log &log, const Ethernet::Address & addr)
{
    String str;

    str << Number::Hex;

    for (Size i = 0; i < sizeof(Ethernet::Address); i++)
        str << addr.addr[i] << ":";

    log.append(*str);

    return log;
}
