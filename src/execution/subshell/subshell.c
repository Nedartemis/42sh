#include "subshell.h"

#include <err.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "run_ast.h"

pid_t fork_subshell(struct ast *ast_list)
{
    pid_t pid = fork();
    if (pid < 0)
    { // fork failed
        errx(3, "execute_subshell: fork failed");
    }
    else if (pid > 0)
    { // case parent
        return pid;
    }

    // executes
    exit(run_ast(ast_list));
}

int execute_subshell(struct ast *ast_list)
{
    if (!ast_list)
        return 0;

    int status;
    pid_t pid = fork_subshell(ast_list);
    waitpid(pid, &status, 0);

    return WEXITSTATUS(status);
}
