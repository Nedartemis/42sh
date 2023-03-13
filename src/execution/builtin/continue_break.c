#include <err.h>
#include <string.h>

#include "builtin.h"
#include "ope_string.h"
#include "sh_instance.h"

static void set_num(char *args[], struct output_builtin *ob,
                    enum BuiltinType type)
{
    int nb = 1;
    if (args[0]) // more than one argument
    {
        if (!is_number(args[0]))
        {
            ob->return_code = 10;
            return;
        }

        nb = atoi(args[0]);
    }

    struct sh *sh = sh_instance_get();
    if (nb > sh->nbloop)
        sh->nbtomanage = sh->nbloop;
    else
        sh->nbtomanage = nb;

    // printf("n = %d ; %d ; %d\n", sh->nbtomanage, sh->nbloop, nb);
    if (sh->nbtomanage > 0)
        sh->builtin_type = type;
}

void my_continue(char *args[], struct output_builtin *ob)
{
    set_num(args, ob, CONTINUE);
}

void my_break(char *args[], struct output_builtin *ob)
{
    set_num(args, ob, BREAK);
}
