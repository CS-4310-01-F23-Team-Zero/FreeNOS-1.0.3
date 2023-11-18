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
#include <stdlib.h> 
#include <ProcessClient.h>
#include <ProcessManager.h>
#include "Renice.h"

Renice::Renice(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Alters the priority of a running process");
    parser().registerPositional("PRIORITY", "priority to be set for process");
    parser().registerPositional("PID", "Process ID of process whose priority will be changed");
    
    parser().registerFlag('n', "priorityFlag", "priority to be set for process");
}

Renice::Result Renice::exec()
{
    u8 p = atoi(arguments().get("PRIORITY"));
    ProcessID pid = atoi(arguments().get("PID"));
    
    //String out;
    //out << atoi(arguments().get("PID"));
    //char line[128];
    //out << line;
    //out<< atoi(arguments().get("PRIORITY"));
    //out << line;
    //write(1, *out, out.length());
    ProcessClient process;
    const ProcessClient::Result result = process.newPrio(pid, p);
    
    if (result == ProcessClient::Success) {
        return Success;
    }
    else if (result == ProcessClient::IOError) {
        String e = "IOError: priority level range 1-5\n";
        char line[128];
        e << line;
        write(1, *e, e.length());

    }
    {
        return NotFound;
    }
    
    
}
