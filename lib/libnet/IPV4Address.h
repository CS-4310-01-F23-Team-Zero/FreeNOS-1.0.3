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

#ifndef __LIB_LIBNET_IPV4ADDRESS_H
#define __LIB_LIBNET_IPV4ADDRESS_H

#include <Types.h>
#include "File.h"
#include "IPV4.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libnet
 * @{
 */

/**
 * IPV4 address file.
 */
class IPV4Address : public File
{
  public:

    /**
     * Constructor
     *
     * @param inode Inode number
     * @param ipv4 IPV4 object pointer
     */
    IPV4Address(const u32 inode,
                IPV4 *ipv4);

    /**
     * Destructor
     */
    virtual ~IPV4Address();

    /**
     * Read IPV4 address
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

    /**
     * Set new IPV4 address
     *
     * @param buffer Input/Output buffer to input bytes from.
     * @param size Maximum number of bytes to write on input.
     *             On output, the actual number of bytes written.
     * @param offset Offset inside the file to start writing.
     *
     * @return Result code
     */
    virtual FileSystem::Result write(IOBuffer & buffer,
                                     Size & size,
                                     const Size offset);

  private:

    /** IPV4 object pointer */
    IPV4 *m_ipv4;
};

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBNET_IPV4ADDRESS_H */
