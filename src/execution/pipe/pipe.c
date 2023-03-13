#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ast.h"
#include "ast_struct.h"
#include "err.h"
#include "list.h"

enum pipe
{
    LEFT = 0,
    RIGHT = 1
};

static pid_t exec_fork_pipe(struct ast *ast, int fds[2], enum pipe side)
{
    pid_t pid = fork();
    if (pid < 0)
        errx(3, "exec_fork_pipe : fork didn't work");
    if (pid != 0)
        return pid;

    if (close(fds[side]) == -1)
    {
        errx(3, "fail to close side");
    }

    int error_code;
    if (side == LEFT)
    {
        error_code = dup2(fds[RIGHT], STDOUT_FILENO);
        if (error_code == -1)
            errx(3, "exec_fork_pipe : dup2 case LEFT didn't work");
        close(fds[RIGHT]);
    }
    else
    {
        error_code = dup2(fds[LEFT], STDIN_FILENO);
        if (error_code == -1)
            errx(3, "exec_fork_pipe : dup2 case RIGHT didn't work");
        close(fds[LEFT]);
    }

    int return_code = ast->vtable->run(ast);
    exit(return_code);
}

int exec_pipe(struct ast *ast_left, struct ast *ast_right)
{
    int fds[2];

    int error = pipe(fds);
    if (error == -1)
        errx(1, "exec_pipe : fail to create pipe");

    pid_t pid_left = exec_fork_pipe(ast_left, fds, LEFT);
    pid_t pid_right = exec_fork_pipe(ast_right, fds, RIGHT);

    close(fds[0]);
    close(fds[1]);

    int status;
    waitpid(pid_left, &status, 0);
    error = WEXITSTATUS(status);

    waitpid(pid_right, &status, 0);
    error = WEXITSTATUS(status);
    if (error == 127)
        errx(127, "exec_pipe : command not found");

    return error;
}
