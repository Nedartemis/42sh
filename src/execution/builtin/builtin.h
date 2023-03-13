#ifndef BUILTIN_H
#define BUILTIN_H

#include <stdbool.h>

#define TRUE 0
#define FALSE 1

#include "vector.h"

struct output_builtin
{
    int return_code;
    struct vector *error;
    struct vector *outpt;
};

// ----------------- my_echo.d
void my_echo(char *args[], struct output_builtin *ob);

// ----------------- output_builtin.h

// check whether what's in 'string' is a builtin
// function or not
bool is_builtin(char *string);
int run_builtin(char *words[]); // run the associated builtin 'o'

// ------------------ true_false.c
void my_true_false(char *args[], struct output_builtin *ob, bool b);

// ------------------ my_exit.c
void my_exit(char *args[], struct output_builtin *ob);

// ------------------ continue_break.c
void my_continue(char *args[], struct output_builtin *ob);
void my_break(char *args[], struct output_builtin *ob);

// ------------------ my_cd.c
void my_cd(char *args[], struct output_builtin *ob);

// ------------------ unset.c
void my_unset(char *args[], struct output_builtin *ob);

// ------------------ my_export.c
void my_export(char *args[], struct output_builtin *ob);
// ------------------ my_cd.c
void my_cd(char *args[], struct output_builtin *ob);
// ------------------ my_alias.c
void my_alias(char *args[], struct output_builtin *ob);
void my_unalias(char *args[], struct output_builtin *ob);

// ------------------ dot.c
void dot(char *arg, struct output_builtin *o);
#endif /* ! BUILTIN_H */
