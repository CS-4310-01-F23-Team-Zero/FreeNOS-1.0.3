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

#include <FreeNOS/System.h>
#include <FreeNOS/ProcessEvent.h>
#include <FreeNOS/ProcessManager.h>
#include <MemoryBlock.h>
#include "VMShare.h"
#include "ProcessID.h"

API::Result VMShareHandler(const ProcessID procID,
                           const API::Operation op,
                           ProcessShares::MemoryShare *share)
{
    ProcessManager *procs = Kernel::instance()->getProcessManager();
    Process *proc = ZERO;
    API::Result ret = API::Success;

    DEBUG("");

    // Find the given process
    if (procID == SELF)
    {
        if (op != API::Read)
            return API::InvalidArgument;
        else
            proc = procs->current();
    }
    else if (op != API::Delete && !(proc = procs->get(procID)))
    {
        return API::NotFound;
    }

    switch (op)
    {
        case API::Create:
        {
            switch (procs->current()->getShares().createShare(proc->getShares(), share))
            {
                case ProcessShares::Success: return API::Success;
                case ProcessShares::AlreadyExists: return API::AlreadyExists;
                case ProcessShares::DetachInProgress: return API::TemporaryUnavailable;
                default: return API::IOError;
            }
            break;
        }

        case API::Read:
            if (procs->current()->getShares().readShare(share) != ProcessShares::Success)
            {
                ret = API::IOError;
            }
            break;

        case API::Delete:
            if (procs->current()->getShares().removeShares(procID) != ProcessShares::Success)
                ret = API::IOError;
            break;

        default:
            return API::InvalidArgument;
    }

    // Done
    return ret;
}
