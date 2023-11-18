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

#ifndef __SERVER_NETWORK_LOOPBACK_H
#define __SERVER_NETWORK_LOOPBACK_H

#include <Types.h>
#include <NetworkDevice.h>
#include <Ethernet.h>

/**
 * @addtogroup server
 * @{
 *
 * @addtogroup loopback
 * @{
 */

/**
 * Loopback network device implementation.
 */
class Loopback : public NetworkDevice
{
  public:

    /**
     * Constructor
     *
     * @param inode Inode number
     * @param server NetworkServer reference
     */
    Loopback(const u32 inode,
             NetworkServer &server);

    /**
     * Destructor
     */
    virtual ~Loopback();

    /**
     * Initialize the device
     *
     * @return Result code
     */
    virtual FileSystem::Result initialize();

    /**
     * Read ethernet address.
     *
     * @param address Ethernet address reference for output
     *
     * @return Result code
     */
    virtual FileSystem::Result getAddress(Ethernet::Address *address);

    /**
     * Set ethernet address
     *
     * @param address New ethernet address to set
     *
     * @return Result code
     */
    virtual FileSystem::Result setAddress(const Ethernet::Address *address);

    /**
     * Transmit one network packet
     *
     * @param pkt Network packet buffer
     */
    virtual FileSystem::Result transmit(NetworkQueue::Packet *pkt);

  private:

    /** Ethernet address for the loopback device */
    Ethernet::Address m_address;
};

/**
 * @}
 * @}
 */

#endif /* __SERVER_NETWORK_LOOPBACK_H */
