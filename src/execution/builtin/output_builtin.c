#include <stdio.h>
#include <string.h>

#include "builtin.h"
#include "list.h"
#include "null_array.h"
#include "tools.h"
#include "vector.h"

#define BUFFER_SIZE 1024

static struct output_builtin *output_builtin_init(void)
{
    struct output_builtin *o = calloc(1, sizeof(struct output_builtin));
    o->error = vector_init(BUFFER_SIZE);
    o->outpt = vector_init(BUFFER_SIZE);

    return o;
}

static void output_builtin_destroy(struct output_builtin *o)
{
    if (!o)
        return;
    if (o->outpt)
        vector_destroy(o->outpt);
    if (o->error)
        vector_destroy(o->error);

    free(o);
}

bool is_builtin(char *string)
{
    if (!string)
        return false;

    static const char *arr[] = {
        "echo",   "true", "false", "exit",    "continue", "break", "unset",
        "export", "cd",   "alias", "unalias", ".",        NULL,
    };

    return array_string_in(arr, string);
}

int run_builtin(char *words[])
{
    char *command = words[0];
    char **args = words + 1;
    struct output_builtin *o = output_builtin_init();

    if (strcmp(command, "echo") == 0)
    {
        my_echo(args, o);
    }
    else if (strcmp(command, "true") == 0)
    {
        my_true_false(args, o, true);
    }
    else if (strcmp(command, "false") == 0)
    {
        my_true_false(args, o, false);
    }
    else if (strcmp(command, "exit") == 0)
    {
        my_exit(args, o);
    }
    else if (strcmp(command, "continue") == 0)
    {
        my_continue(args, o);
    }
    else if (strcmp(command, "break") == 0)
    {
        my_break(args, o);
    }
    else if (strcmp(command, "cd") == 0)
    {
        my_cd(args, o);
    }
    else if (strcmp(command, "unset") == 0)
    {
        my_unset(args, o);
    }
    else if (strcmp(command, "export") == 0)
    {
        my_export(args, o);
    }
    else if (strcmp(command, ".") == 0)
    {
        dot(args[0], o);
    }
    else if (strcmp(command, "alias") == 0)
    {
        my_alias(args, o);
    }
    else if (strcmp(command, "unalias") == 0)
    {
        my_unalias(args, o);
    }

    if (o->outpt)
        fprintf(stdout, "%s", o->outpt->data);
    if (o->error)
        fprintf(stderr, "%s", o->error->data);

    int return_code = o->return_code;
    output_builtin_destroy(o);

    return return_code;
}
