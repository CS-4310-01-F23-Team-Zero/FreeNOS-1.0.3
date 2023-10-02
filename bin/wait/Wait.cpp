#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h> // Include for waitpid
#include "Wait.h"

Wait::Wait(int argc, char **argv)
    : POSIXApplication(argc, argv)
{
    parser().setDescription("Wait for process");
    parser().registerPositional("PID", "Process ID");
}

Wait::~Wait()
{
}

Wait::Result Wait::exec()
{
    int status;
    pid_t pid;
    
    // get pid from command line
    pid = atoi(arguments().get("PID"));

    if (waitpid(pid, &status, 0) == (pid_t)-1)
    {
        ERROR("Failed to wait: " << strerror(errno));
        return IOError;
    }

    // Done
    return Success;
}

