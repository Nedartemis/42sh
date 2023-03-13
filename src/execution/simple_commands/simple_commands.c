#include "simple_commands.h"

#include <err.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "list.h"
#include "tools.h"

static pid_t exec_fork(char **argv)
{
    pid_t pid = fork();
    if (pid != 0)
    {
        // Parent process
        return pid;
    }

    // Child process

    // Execute 1st arg as command, followed by its args
    execvp(argv[0], argv);
    errx(127, "Error : Simple command does not exist ; errno = %d", errno);
}

int exec_command(char **argv)
{
    // CAREFULL : argv must be null-terminated array
    pid_t pid_arg = exec_fork(argv);

    int status;
    waitpid(pid_arg, &status, 0);
    return WEXITSTATUS(status);
}

int run_simple_command(char *words[])
{
    int return_code = exec_command(words);
    return return_code;
}
