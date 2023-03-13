#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "dico_word.h"
#include "expansion.h"
#include "expansion_quote.h"
#include "list.h"
#include "sh_instance.h"
#include "special_var.h"
#include "tools.h"
#include "vector.h"

static bool is_char_var(char c)
{
    bool boo = isalnum(c) || c == '_';
    return boo;
}

static char *get_name_braket(char *input, size_t *i)
{
    assert(input[*i] == '{');
    *i += 1;

    int size = 0;
    for (; input[*i + size] != '}'; size++)
        continue;

    if (size == 0)
    {
        *i += 1;
        return NULL;
    }

    char *name = strndup(input + *i, size);
    *i += size + 1;
    return name;
}

static char *get_name_without_bracket(char *input, size_t *i)
{
    if (is_special_char(input[*i]))
    {
        char *var_name = strndup(input + *i, 1);
        *i += 1;
        return var_name;
    }

    struct vector *v = vector_init(16);

    for (; true;)
    {
        char c = input[*i];
        if (is_char_var(c))
        {
            vector_append(v, c);
            *i += 1;
            continue;
        }
        else
            break;
    }

    if (v->size == 0)
    {
        vector_destroy(v);
        return NULL;
    }

    char *var_name = vector_to_string(v);
    vector_destroy(v);
    return var_name;
}

static char *get_name(char *input, size_t *i)
{
    if (input[*i] == '{')
    {
        return get_name_braket(input, i);
    }
    return get_name_without_bracket(input, i);
}

static struct list *substitution(char *name)
{
    if (is_special_var(name))
    {
        struct list *return_list = special_substitution(name);
        free(name);
        return return_list;
    }

    // get the value from dico
    const char *value = dico_word_get(sh_instance_get()->dv, name);
    free(name);

    // create and return list words of one single word
    struct list *words = list_init();
    if (value)
        list_add(words, strdup(value));

    return words;
}

struct list *expansion_var(char *input, size_t *i)
{
    assert(input[*i] == '$');
    *i += 1;

    char *var_name = get_name(input, i);
    if (var_name)
        return substitution(var_name);

    struct list *words = list_init();
    list_add(words, strdup("$"));
    return words;
}
