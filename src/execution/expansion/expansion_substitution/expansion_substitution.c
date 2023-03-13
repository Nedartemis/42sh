#include <assert.h>
#include <stdbool.h>
#include <stdio.h>

#include "expansion_cmd_subst.h"
#include "expansion_var.h"
#include "field_splitting.h"
#include "list.h"
#include "ope_string.h"

struct list *expansion_substitution(char *input, size_t *i,
                                    bool need_field_splitting)
{
    assert(input[*i] == '$' || input[*i] == '`');

    struct list *words;
    if (input[*i] == '$')
    {
        if (input[*i + 1] == '(')
        {
            words = expansion_dollar_par(input, i);
        }
        else
        {
            words = expansion_var(input, i);
        }
    }
    else
    {
        words = expansion_backquote(input, i);
    }

    if (need_field_splitting)
    {
        // printf("before : ");
        // list_print(words, print_string);
        // printf("\n");
        field_splitting(&words);
        // printf("after : ");
        // list_print(words, print_string);
        // printf("\n");
    }

    return words;
}

bool beginning_substitution(char c)
{
    return c == '$' || c == '`';
}
