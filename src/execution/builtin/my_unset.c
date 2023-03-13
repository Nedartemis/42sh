#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"
#include "dico_func.h"
#include "dico_word.h"
#include "sh.h"
#include "sh_instance.h"

static void unset_var(const char *key, bool v, bool f)
{
    struct sh *sh = sh_instance_get();
    if (v)
    {
        dico_word_remove(sh->dv, key);
        unsetenv(key);
    }
    else if (f)
    {
        dico_func_remove(sh->df, key);
    }
    else
    {
        if (getenv(key))
        {
            unsetenv(key);
        }
        else if (dico_word_in(sh->dv, key))
        {
            dico_word_remove(sh->dv, key);
        }
        else
            dico_func_remove(sh->df, key);
    }
}

void my_unset(char *args[], struct output_builtin *ob)
{
    if (!args)
        return;

    bool v = false;
    bool f = false;

    int i = 0;
    if (args[0][0] == '-')
    {
        if (strcmp(args[0], "-v") == 0)
            v = true;
        else if (strcmp(args[0], "-f") == 0)
            f = true;
        i += 1;
    }

    for (; args[i]; i++)
    {
        unset_var(args[i], v, f);
    }

    ob->return_code = 0;
}
