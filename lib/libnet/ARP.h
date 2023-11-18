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

#ifndef __LIB_LIBNET_ARP_H
#define __LIB_LIBNET_ARP_H

#include <Types.h>
#include <HashTable.h>
#include <Timer.h>
#include <KernelTimer.h>
#include "Ethernet.h"
#include "IPV4.h"
#include "NetworkProtocol.h"

class ARPSocket;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * Address Resolution Protocol
 */
class ARP : public NetworkProtocol
{
  private:

    /** Maximum number of retries for ARP lookup */
    static const Size MaxRetries = 3;

    /**
     * ARP table cache entry
     */
    typedef struct ARPCache
    {
        Ethernet::Address ethAddr;
        Timer::Info time;
        Size retryCount;
        bool valid;
    }
    ARPCache;

  public:

    /**
     * ARP hardware types
     */
    enum HardwareType
    {
        Ethernet = 1
    };

    /**
     * ARP inter-network protocol types.
     */
    enum ProtocolType
    {
        IPV4 = 0x0800
    };

    /**
     * ARP message types (operation codes)
     */
    enum Operation
    {
        Request = 1,
        Reply   = 2
    };

    /**
     * ARP network packet header.
     *
     * This header is prepended to every
     * ARP packet send on the actual physical link.
     */
    typedef struct Header
    {
        u16 hardwareType;
        u16 protocolType;
        u8  hardwareLength;
        u8  protocolLength;
        u16 operation;

        Ethernet::Address etherSender;
        IPV4::Address     ipSender;

        Ethernet::Address etherTarget;
        IPV4::Address     ipTarget;
    }
    PACKED Header;

  public:

    /**
     * Constructor
     *
     * @param server Reference to the NetworkServer instance
     * @param device Reference to the NetworkDevice instance
     * @param parent Parent upper-layer protocol
     */
    ARP(NetworkServer &server,
        NetworkDevice &device,
        NetworkProtocol &parent);

    /**
     * Destructor
     */
    virtual ~ARP();

    /**
     * Perform initialization.
     *
     * @return Result code
     */
    virtual FileSystem::Result initialize();

    /**
     * Set IPV4 instance
     *
     * @param ip IPV4 instance
     */
    void setIP(::IPV4 *ip);

    /**
     * Lookup Ethernet address for an IP
     *
     * @param ipAddr Input IP address to lookup
     * @param ethAddr Output Ethernet address when found
     *
     * @return Result code
     */
    FileSystem::Result lookupAddress(const IPV4::Address *ipAddr,
                                     Ethernet::Address *ethAddr);

    /**
     * Send ARP request
     *
     * @param address IPV4 address to lookup
     *
     * @return Result code
     */
    FileSystem::Result sendRequest(const IPV4::Address address);

    /**
     * Send ARP reply
     *
     * @param ethaddr Ethernet address to send reply to
     * @param ipAddr IP address of the origin
     *
     * @return Result code
     */
    FileSystem::Result sendReply(const Ethernet::Address *ethaddr,
                                 const IPV4::Address ipAddr);

    /**
     * Process incoming network packet.
     *
     * @param pkt Incoming packet pointer
     * @param offset Offset for processing
     *
     * @return Result code
     */
    virtual FileSystem::Result process(const NetworkQueue::Packet *pkt,
                                       const Size offset);

  private:

    /**
     * Insert a new entry to the ARP cache
     *
     * @param ipAddr IP address to add
     *
     * @return ARPCache object pointer
     */
    ARPCache * insertCacheEntry(const IPV4::Address ipAddr);

    /**
     * Retrieve cache entry by IP
     *
     * @param ipAddr IP address to lookup
     *
     * @return ARPCache object pointer or ZERO if not found
     */
    ARPCache * getCacheEntry(const IPV4::Address ipAddr);

    /**
     * Update cache entry
     *
     * @param ipAddr IP address for update
     * @param ethAddr Ethernet address for update
     */
    void updateCacheEntry(const IPV4::Address ipAddr,
                          const Ethernet::Address *ethAddr);

  private:

    /** The single ARP socket */
    ARPSocket *m_sock;

    /** IPV4 instance object */
    ::IPV4 *m_ip;

    /** Contains a cached mapping from IP to Ethernet addresses */
    HashTable<IPV4::Address, ARPCache *> m_cache;

    /** Provides access to the kernel timer */
    KernelTimer m_kernelTimer;
};

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBNET_ARP_H */
