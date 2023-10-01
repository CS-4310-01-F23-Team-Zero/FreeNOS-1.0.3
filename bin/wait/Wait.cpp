// Wait.cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "Wait.h"

Wait::Wait(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Wait for a background process to finish");
    parser().registerPositional("PID", "Process ID to wait for");
}

Wait::~Wait()
{
}

Wait::Result Wait::exec()
{
    int pid = 0;

    // Convert input to process ID
    if ((pid = atoi(arguments().get("PID"))) <= 0)
    {
        ERROR("invalid PID: " << arguments().get("PID"));
        return InvalidArgument;
    }

    // Use waitpid to wait for the specified process to exit
    int status;
    int result = waitpid(pid, &status, 0);

    if (result == -1)
    {
        ERROR("failed to wait for PID " << pid << ": " << strerror(errno));
        return IOError;
    }

    if (WIFEXITED(status))
    {
        INFO("Process " << pid << " exited with status " << WEXITSTATUS(status));
    }
    else
    {
        INFO("Process " << pid << " did not exit normally");
    }

    // Done
    return Success;
}

