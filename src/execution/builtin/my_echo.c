#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "builtin.h"
#include "tools.h"
#include "vector.h"

bool is_there_unrec_opt(char *str)
{
    for (int i = 1; str[i]; i++)
    {
        switch (str[i])
        {
        case 'n':
            break;
        case 'e':
            break;
        case 'E':
            break;
        default:
            return false;
        }
    }
    return true;
}

int setup_opt(char **args, bool *n, bool *e)
{
    int i = 0;

    // options management
    for (; args[i] != NULL; i++)
    {
        char *str = args[i];

        if (str[0] != '-')
            return i;

        if (!is_there_unrec_opt(str))
            return i;

        for (int j = 1; str[j]; j++)
        {
            switch (str[j])
            {
            case 'n':
                *n = true;
                break;
            case 'e':
                *e = true;
                break;
            default: // E
                *e = false;
                break;
            }
        }
    }

    return i;
}

void my_echo(char *args[], struct output_builtin *ob)
{
    bool n_opt = false; // disable last newline
    bool e_opt = false; // interpret escaped characters

    int i = setup_opt(args, &n_opt, &e_opt);

    for (; args[i]; i++)
    {
        char *p = args[i];
        for (; *p; p++)
        {
            if (*p == '\\' && *(p++))
            {
                switch (*p)
                {
                case 'n':
                    if (e_opt)
                        vector_append(ob->outpt, '\n');
                    else
                    {
                        vector_append(ob->outpt, '\\');
                        vector_append(ob->outpt, 'n');
                    }
                    break;

                case 't':
                    if (e_opt)
                        vector_append(ob->outpt, '\t');
                    else
                    {
                        vector_append(ob->outpt, '\\');
                        vector_append(ob->outpt, 't');
                    }
                    break;

                case '\\':
                    vector_append(ob->outpt, '\\');
                    if (!e_opt)
                        vector_append(ob->outpt, '\\');
                    break;

                default: // not a handled escaped character
                    p--;
                    vector_append(ob->outpt, *p);
                    break;
                }
            }
            else
            {
                vector_append(ob->outpt, *p);
            }

            // last character
            if (*(p + 1) == '\0' && args[i + 1])
                vector_append(ob->outpt, ' ');
        }
    }

    if (!n_opt)
    {
        vector_append(ob->outpt, '\n');
    }
}
