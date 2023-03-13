#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "builtin.h"
#include "err.h"

#define PATH_MAX 1024

static void cd_option(struct output_builtin *ob)
{
    char *tmp = getenv("OLDPWD");

    // Case program starts, no oldpwd
    if (!tmp)
    {
        ob->return_code = 1;
        perror("cd_option : OLDPWD not set");
        return;
    }

    chdir(tmp);
    printf("%s\n", tmp);
    setenv("OLDPWD", getenv("PWD"), 1);
    setenv("PWD", tmp, 1);
    return;
}

/*
static char *concat_path(char *adding, char *old)
{
    if (!adding[0] || adding[0] == '/')
    {
        //printf("size of adding : %ld\n", strlen(adding));
        char *dest = calloc(1, strlen(adding) + 1);
        strcat(dest, adding);
        return dest;
    }

    // Case we need to concate subdir to dir

    char *dest = calloc(1, strlen(adding) + strlen(old) + 2);
    strcat(dest, old);
    strcat(dest, "/");
    return strcat(dest, adding);
}*/

static void cd_without_option(char *new_path, struct output_builtin *ob)
{
    char *owd = getenv("PWD");
    char cwd[PATH_MAX];

    // Case pwd not set
    if (!owd)
    {
        getcwd(cwd, PATH_MAX);
        setenv("OLDPWD", cwd, 1);
    }
    else
        setenv("OLDPWD", owd, 1);

    // Change working dir
    if (chdir(new_path) == -1)
    {
        perror("cd_without_option : file not found");
        ob->return_code = 1;
        return;
    }

    // char *absolute_path = concat_path(new_path, getenv("OLDPWD"));

    getcwd(cwd, PATH_MAX);
    setenv("PWD", cwd, 1);

    // free(absolute_path);
    ob->return_code = 0;
}

static void exec_cd(char *new_path, struct output_builtin *ob)
{
    // Case cd -
    // if (strcmp(new_path, "-") == 0)
    if (new_path && new_path[0] && new_path[0] == '-')
    {
        cd_option(ob);
    }
    else
    {
        cd_without_option(new_path, ob);
    }
}

void my_cd(char *args[], struct output_builtin *ob)
{
    if (!args || !args[0])
    {
        ob->return_code = 3;
        return;
    }

    // printf("args : %s\n", args[0]);
    exec_cd(args[0], ob);
}
