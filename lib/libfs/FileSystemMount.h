/*
 * Copyright (C) 2009 Niek Linnenbank
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

#ifndef __LIB_LIBFS_FILESYSTEMMOUNT_H
#define __LIB_LIBFS_FILESYSTEMMOUNT_H

#include <Types.h>
#include "FileSystemPath.h"

/**
 * @addtogroup lib
 * @{
 *
 * @addtogroup libposix
 * @{
 */

/**
 * Represents a mounted filesystem.
 */
typedef struct FileSystemMount
{
    /** Path of the mount. */
    char path[FileSystemPath::MaximumLength];

    /** Server which is responsible for the mount. */
    ProcessID procID;

    /** Mount options. */
    ulong options;
}
FileSystemMount;

/**
 * @}
 * @}
 */

#endif /* __LIB_LIBFS_FILESYSTEMMOUNT_H */
