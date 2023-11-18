/*
* Copyright(C) 2015 Niek Linnenbank
*
*This program is free software : you can redistribute it and /or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
*(at your option) any later version.
*
*This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see < http://www.gnu.org/licenses/>.
*/


#include <Log.h>
#include "Kernel.h"
#include "Scheduler.h"


Scheduler::Scheduler()
{
    DEBUG("");
}


Size Scheduler::count(u8 priority) const
{
    int p = (static_cast<int>(priority));
    if (p == 1)
        return one_queue.count();
    else if (p == 2)
        return two_queue.count();
    else if (p == 3)
        return three_queue.count();
    else if (p == 4)
        return four_queue.count();
    else if (p == 5)
        return five_queue.count();
}


Scheduler::Result Scheduler::enqueue(Process * proc, bool ignoreState, u8 priority)
{
    int p = static_cast<int>(priority);
    if (proc->getState() != Process::Ready && !ignoreState)
    {
        ERROR("process ID " << proc->getID() << " not in Ready state");
        return InvalidArgument;
    }
    if (p == 1)
        one_queue.push(proc);
    else if (p == 2)
        two_queue.push(proc);
    else if (p == 3)
        three_queue.push(proc);
    else if (p == 4)
        four_queue.push(proc);
    else if (p == 5)
        five_queue.push(proc);

    return Success;
}


Scheduler::Result Scheduler::dequeue(Process * proc, bool ignoreState, u8 priority)
{
    int prio = static_cast<int>(priority);
    if (proc->getState() == Process::Ready && !ignoreState)
    {
        ERROR("process ID " << proc->getID() << " is in Ready state");
        return InvalidArgument;
    }
    

    Size count;
    if (prio == 1)
    {
        count = one_queue.count();
        // Traverse the Queue to remove the Process
        for (Size i = 0; i < count; i++)
        {
            Process* p = one_queue.pop();


            if (p == proc)
                return Success;
            else
                one_queue.push(p);
        }
    }
    else if (prio == 2)
    {
        count = two_queue.count();
        // Traverse the Queue to remove the Process
        for (Size i = 0; i < count; i++)
        {
            Process* p = two_queue.pop();
            /*out<<p->getID()<<"\n";
            write(1, *out, out.length());*/

            INFO("#" << p->getID()<<"\n");

            if (p == proc)
                return Success;
            else
                two_queue.push(p);
        }
        

    }
    else if (prio == 3)
    {
        count = three_queue.count();
        // Traverse the Queue to remove the Process
        for (Size i = 0; i < count; i++)
        {
            Process* p = three_queue.pop();


            if (p == proc)
                return Success;
            else
                three_queue.push(p);
        }
    }
    else if (prio == 4)
    {
        count = four_queue.count();
        // Traverse the Queue to remove the Process
        for (Size i = 0; i < count; i++)
        {
            Process* p = four_queue.pop();


            if (p == proc)
                return Success;
            else
                four_queue.push(p);
        }
    }
    else if (prio == 5)
    {
        count = five_queue.count();
        // Traverse the Queue to remove the Process
        for (Size i = 0; i < count; i++)
        {
            Process* p = five_queue.pop();


            if (p == proc)
                return Success;
            else
                five_queue.push(p);
        }
    }


    FATAL("process ID " << proc->getID() << " is not in the schedule p: "<<prio
    <<" two_queue: "<< two_queue.count()<<" three_queue: "<<three_queue.count());
    return InvalidArgument;
}


Process* Scheduler::select()
{
    if (five_queue.count() > 0)
    {
        Process* p = five_queue.pop();
        five_queue.push(p);
        return p;
    }
    if (four_queue.count() > 0)
    {
        Process* p = four_queue.pop();
        four_queue.push(p);


        return p;
    }
    if (three_queue.count() > 0)
    {
        Process* p = three_queue.pop();
        three_queue.push(p);


        return p;
    }
    if (two_queue.count() > 0)
    {
        Process* p = two_queue.pop();
        two_queue.push(p);


        return p;
    }
    if (one_queue.count() > 0)
    {
        Process* p = one_queue.pop();
        one_queue.push(p);


        return p;
    }


    return (Process*)NULL;
}
