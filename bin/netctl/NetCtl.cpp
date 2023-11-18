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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <MemoryBlock.h>
#include <NetworkClient.h>
#include <NetworkSocket.h>
#include <IPV4.h>
#include <ICMP.h>
#include <Ethernet.h>
#include <FileSystemClient.h>
#include "NetCtl.h"

NetCtl::NetCtl(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("control network devices");
    parser().registerPositional("ARGS", "optional key=value arguments", 0);
}

NetCtl::~NetCtl()
{
}

NetCtl::Result NetCtl::initialize()
{
    DEBUG("");
    return Success;
}

NetCtl::Result NetCtl::exec()
{
    Size numberOfMounts = 0;

    DEBUG("");

    // Make a list of network devices
    // Get a list of mounts
    FileSystemClient filesystem;
    FileSystemMount *mounts = filesystem.getFileSystems(numberOfMounts);

    // Find closest matching device
    for (Size i = 0; i < numberOfMounts; i++)
    {
        if (mounts[i].path[0] && strncmp(mounts[i].path, "/network/", 9) == 0)
        {
            showDevice(mounts[i].path + 9);
        }
    }
    return Success;
}

NetCtl::Result NetCtl::showDevice(const char *deviceName)
{
    DEBUG("");

    String ipv4, ether, out;
    ether << "/network/" << deviceName << "/ethernet/address";
    ipv4  << "/network/" << deviceName << "/ipv4/address";
    out   << deviceName << " ipv4 ";

    // read the ipv4/address file
    int fd = open(*ipv4, O_RDONLY);
    int r;

    if (fd != -1)
    {
        IPV4::Address ipAddr;

        r = read(fd, &ipAddr, sizeof(ipAddr));
        if (r != -1)
        {
            out << IPV4::toString(ipAddr);
        }
        close(fd);
    }
    out << " ether ";

    fd = open(*ether, O_RDONLY);
    if (fd != -1)
    {
        Ethernet::Address etherAddress;

        r = read(fd, &etherAddress, sizeof(etherAddress));
        if (r != -1)
        {
            out << Ethernet::toString(etherAddress);
        }
        close(fd);
    }

    printf("%s\r\n", *out);
    return Success;
}
