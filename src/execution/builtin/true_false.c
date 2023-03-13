#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"

void my_true_false(char *args[], struct output_builtin *ob, bool b)
{
    (void)args;
    /*
    //Takes last arg to handle "false; true" situation
    if(args and *args)
    {
        size_t size_args = sizeof(args) / sizeof(args[0]);
        b = args[size_args-1];
    }*/

    if (b) // true
    {
        ob->return_code = 0;
    }
    else // false
    {
        ob->return_code = 1;
    }
}
