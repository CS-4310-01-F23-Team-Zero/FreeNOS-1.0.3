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

#ifndef __KERNEL_PROCESS_MANAGER_H
#define __KERNEL_PROCESS_MANAGER_H

#include <Types.h>
#include <MemoryMap.h>
#include <Vector.h>
#include <List.h>
#include <Queue.h>
#include "Process.h"

/* Forward declarations */
class Scheduler;

/**
 * @addtogroup kernel
 * @{
 */

/**
 * Maximum number of processes.
 */
#define MAX_PROCS 1024

/**
 * Represents a process which may run on the host.
 */
class ProcessManager
{
  public:

    /**
     * Result code
     */
    enum Result
    {
        Success,
        InvalidArgument,
        IOError,
        WakeupPending,
        AlreadyExists
    };

  public:

    /**
     * Constructor function.
     */
    ProcessManager();

    /**
     * Destructor function.
     */
    virtual ~ProcessManager();

    /**
     * Create a new Process.
     *
     * @param entry Process executable entry point
     * @param map Memory mapping
     * @param readyToRun True to immediately run the Process or false to sleep.
     * @param privileged True to create a privileged Process
     *
     * @return Process pointer on success or ZERO on failure
     */
    Process * create(const Address entry,
                     const MemoryMap &map,
                     const bool readyToRun = false,
                     const bool privileged = false);

    /**
     * Retrieve a Process by it's ID.
     *
     * @param id ProcessID number.
     *
     * @return Pointer to the appropriate process or ZERO if not found.
     */
    Process * get(const ProcessID id);

    /**
     * Remove a Process.
     */
    void remove(Process *proc, const uint exitStatus = 0);

    /**
     * Schedule next process to run.
     *
     * @return Result code
     */
    Result schedule();

    /**
     * Let current Process wait for another Process to terminate.
     *
     * @param proc Process pointer
     *
     * @return Result code
     */
    Result wait(Process *proc);

    /**
     * Remove given Process from the Scheduler
     *
     * @param proc Process pointer
     *
     * @return Result code
     */
    Result stop(Process *proc);

    /**
     * Resume scheduling of the given Process.
     *
     * @param proc Process pointer
     *
     * @return Result code
     */
    Result resume(Process *proc);

    /**
     * Restart execution of a Process at the given entry point.
     *
     * @param proc Process pointer
     * @param entry Address to begin execution.
     */
    Result reset(Process *proc, const Address entry);

    /**
     * Let current Process sleep until a timer expires or wakeup occurs.
     *
     * @param timer Timer on which the process must be woken up (if expired), or ZERO for no limit
     * @param ignoreWakeups True to enter Sleep state regardless of pending wakeups
     *
     * @return Result code
     */
    Result sleep(const Timer::Info *timer = 0, const bool ignoreWakeups = false);

    /**
     * Take Process out of Sleep state and mark ready for execution.
     *
     * @param proc Process pointer
     *
     * @return Result code
     */
    Result wakeup(Process *proc);

    /**
     * Raise kernel event for a Process
     *
     * @param proc Process pointer
     * @param event Event to raise
     *
     * @return Result code
     */
    Result raiseEvent(Process *proc, const struct ProcessEvent *event);

    /**
     * Register an interrupt notification for a Process.
     *
     * @param proc Process pointer
     * @param vector Interrupt vector number
     *
     * @return Result code
     */
    Result registerInterruptNotify(Process *proc, const u32 vector);

    /**
     * Remove all interrupt notifications for a Process
     *
     * @param proc Process pointer
     *
     * @return Result code
     */
    Result unregisterInterruptNotify(Process *proc);

    /**
     * Raise interrupt notifications for a interrupt vector
     *
     * @param vector Interrupt vector
     *
     * @return Result code
     */
    Result interruptNotify(const u32 vector);

    /**
     * Set the idle process.
     */
    void setIdle(Process *proc);

    /**
     * Current process running. NULL if no process running yet.
     *
     * @return Process pointer
     */
    Process * current();

  private:

    /**
     * Place the given process on the Schedule queue
     *
     * @param proc Process pointer
     * @param ignoreState True to not check for the Process state prior to dequeue.
     *
     * @return Result code
     */
    Result enqueueProcess(Process *proc, const bool ignoreState = false);

    /**
     * Remove the given process on the Schedule queue
     *
     * @param proc Process pointer
     * @param ignoreState True to not check for the Process state prior to dequeue.
     *
     * @return Result code
     */
    Result dequeueProcess(Process *proc, const bool ignoreState = false) const;

  private:

    /** All known Processes. */
    Index<Process, MAX_PROCS> m_procs;

    /** Object which selects processes to run. */
    Scheduler *m_scheduler;

    /** Currently executing process */
    Process *m_current;

    /** Idle process */
    Process *m_idle;

    /** Queue with sleeping processes waiting for a Timer to expire. */
    Queue<Process *, MAX_PROCS> m_sleepTimerQueue;

    /** Interrupt notification list */
    Vector<List<Process *> *> m_interruptNotifyList;
};

/**
 * @}
 */

#endif /* __KERNEL_PROCESS_MANAGER_H */
