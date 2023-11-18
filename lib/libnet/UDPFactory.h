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

#ifndef __LIB_LIBNET_UDPFACTORY_H
#define __LIB_LIBNET_UDPFACTORY_H

#include <File.h>

class UDP;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * User Datagram Protocol (UDP).
 *
 * The UDP factory creates new sockets for applications.
 */
class UDPFactory : public File
{
  public:

    /**
     * Constructor
     *
     * @param inode Inode number
     * @param udp UDP object pointer
     */
    UDPFactory(const u32 inode,
               UDP *udp);

    /**
     * Destructor
     */
    virtual ~UDPFactory();

    /**
     * Create UDP socket
     *
     * @param buffer Input/Output buffer to output bytes to.
     * @param size Maximum number of bytes to read on input.
     *             On output, the actual number of bytes read.
     * @param offset Offset inside the file to start reading.
     *
     * @return Result code
     */
    virtual FileSystem::Result read(IOBuffer & buffer,
                                    Size & size,
                                    const Size offset);

  private:

    /** UDP protocol instance */
    UDP *m_udp;
};

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBNET_UDPFACTORY_H */
