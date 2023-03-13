#include <stdbool.h>

#include "builtin.h"
#include "sh.h"
#include "sh_instance.h"
#include "string.h"

bool contains_slash(char *file)
{
    char *p = file;
    while (*p != '\0')
    {
        if (*p == '/')
            break;
        p++;
    }
    return *p != '\0';
}

void dot(char *arg, struct output_builtin *o)
{
    struct sh *sh = sh_instance_get();

    // setup restoring args later
    int save_argc = sh->argc;
    char *save_argv = sh->argv[1];
    FILE *save_input = sh->parser->lexer->input;

    // true if we're looking into $PATH
    if (!contains_slash(arg))
    {
        char *path_var = strcat(getenv("PATH"), "/");
        arg = strcat(path_var, arg);
    }

    sh->argc = 2;
    sh->argv[1] = arg;

    sh_reset(sh);

    // run shell in the folder
    o->return_code = sh_run(sh);

    // RESET
    sh->argc = save_argc;
    sh->argv[1] = save_argv;
    sh->parser->lexer->input = save_input;

    sh_reset(sh);
}
