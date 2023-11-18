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

#include <Types.h>
#include <Macros.h>
#include <stdio.h>
#include <unistd.h>
#include <ProcessClient.h>
#include "ProcessList.h"

ProcessList::ProcessList(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Output system process list");
    parser().registerFlag('l', "long", "List processes in long output format");
}

ProcessList::Result ProcessList::exec()
{
    const ProcessClient process;
    String out;

    // Print header
    if(!arguments().get("long")) {
        out << "ID  PARENT  USER GROUP STATUS     CMD\r\n";

    }
    else {
        out << "ID  PARENT  USER GROUP PRIORITY STATUS     CMD\r\n";

    }

    // Loop processes
    for (ProcessID pid = 0; pid < ProcessClient::MaximumProcesses; pid++)
    {
        ProcessClient::Info info;

        const ProcessClient::Result result = process.processInfo(pid, info);
        if (result == ProcessClient::Success)
        {
            DEBUG("PID " << pid << " state = " << *info.textState);

            char line[128];
            if (!arguments().get("long")) {
                snprintf(line, sizeof(line),
                    "%3d %7d %4d %5d %10s %32s\r\n",
                    pid, info.kernelState.parent,
                    0, 0, *info.textState, *info.command);

            }
            else {
                snprintf(line, sizeof(line),
                    "%3d %7d %4d %5d %8u %10s %32s\r\n",
                    pid, info.kernelState.parent,
                    0, 0, info.kernelState.priority, *info.textState, *info.command);

            }

            // Output a line
             
            out << line;
        }
    }

    // Output the table
    write(1, *out, out.length());
    return Success;
}
