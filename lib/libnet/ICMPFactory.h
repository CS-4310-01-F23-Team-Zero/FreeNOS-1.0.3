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

#ifndef __LIB_LIBNET_ICMPFACTORY_H
#define __LIB_LIBNET_ICMPFACTORY_H

#include <File.h>
#include "ICMP.h"

class ICMP;

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * Internet Control Message Protocol (ICMP) factory.
 *
 * The ICMP factory creates new sockets for applications.
 */
class ICMPFactory : public File
{
  public:

    /**
     * Constructor
     *
     * @param inode Inode number
     * @param icmp ICMP object pointer
     */
    ICMPFactory(const u32 inode,
                ICMP *icmp);

    /**
     * Destructor
     */
    virtual ~ICMPFactory();

    /**
     * Create ICMP socket
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

    /** ICMP protocol instance */
    ICMP *m_icmp;
};

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBNET_ICMPFACTORY_H */
