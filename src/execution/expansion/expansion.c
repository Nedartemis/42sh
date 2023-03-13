#include "expansion.h"

#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "dico_word.h"
#include "expansion_cmd_subst.h"
#include "expansion_helper.h"
#include "expansion_quote.h"
#include "expansion_substitution.h"
#include "expansion_var.h"
#include "field_splitting.h"
#include "list_helper.h"
#include "null_array.h"
#include "ope_string.h"
#include "sh_instance.h"
#include "special_var.h"
#include "struct.h"
#include "tools.h"

#define BUFFER_SIZE 64

// VAR GLOBALE

// When set_unset == 0 => set
//      set_unset == 1 => unset
static void set_unset_env_var(char *ass_word, int set_unset)
{
    size_t i = 0;
    for (; ass_word[i] != '='; i++)
        continue;
    char *name = strndup(ass_word, i);

    if (set_unset == 0)
    {
        char *value = strdup(ass_word + i + 1);
        char *value_expanded = concat_list_string(expansion(value, false), ' ');

        setenv(name, value_expanded, 1);
        free(value);
        free(value_expanded);
    }
    else
    {
        unsetenv(name);
    }

    free(name);
}

void treat_env_var(struct list *prefixes, int set_unset)
{
    size_t size = list_size(prefixes);

    for (size_t i = 0; i < size; i++)
    {
        void *data = list_get(prefixes, i);
        struct ast_prefix *ap = (struct ast_prefix *)data;

        set_unset_env_var(ap->ass_word, set_unset);
    }
}

int assign_word(const char *ass_word)
{
    size_t i = 0;
    for (; ass_word[i] != '='; i++)
        continue;

    char *name = strndup(ass_word, i);
    char *value = strdup(ass_word + i + 1);

    char *value_expanded = concat_list_string(expansion(value, false), ' ');

    struct sh *sh = sh_instance_get();

    if (is_env_var(name))
    {
        setenv(name, value_expanded, 1);
    }
    else
    {
        dico_word_insert(sh->dv, name, value_expanded);
    }

    free(name);
    free(value);
    free(value_expanded);

    return 0;
}

struct list *expansion(char *input, bool need_field_splitting)
{
    struct list *return_list = list_init();
    struct vector *expanded_word = vector_init(BUFFER_SIZE);
    struct list *new_words;

    for (size_t i = 0; input[i];)
    {
        if (beginning_substitution(input[i]))
        {
            new_words = expansion_substitution(input, &i, need_field_splitting);
            concat_special_list(return_list, new_words);
            continue;
        }

        switch (input[i])
        {
        case '\'':
            expansion_single_quote(input, &i, expanded_word);
            break;
        case '\\':
            expension_escape_char(input, &i, expanded_word);
            break;
            break;
        case '"':
            expension_double_quote(input, &i, expanded_word);
            break;
        default:
            vector_append(expanded_word, input[i]);
            i++;
            break;
        }

        concat_vector_to_last_element(return_list, expanded_word);
    }

    vector_destroy(expanded_word);

    // printf("new word : %s\n", new_word);
    return return_list;
}

static char **put_list_into_arr(struct list *l)
{
    size_t size = list_size(l);
    char **arr_words = calloc(size + 1, sizeof(char *));

    for (size_t i = 0; !list_empty(l); i += 1)
    {
        char *str = (char *)list_pop_front(l);
        arr_words[i] = str;
    }
    arr_words[size] = NULL;

    list_destroy(l, destroy_string);
    return arr_words;
}

struct list *list_expend_to_list(struct list *input_words)
{
    struct list *output_words = list_init();

    size_t input_size = list_size(input_words);

    for (size_t i = 0; i < input_size; i++)
    {
        char *element = (char *)list_get(input_words, i);
        struct list *exp_list = expansion(element, true);
        while (!list_empty(exp_list))
        {
            void *word = list_pop_front(exp_list);
            list_add(output_words, word);
        }

        list_destroy(exp_list, destroy_string);
    }

    return output_words;
}

char **list_expend_to_arr(struct list *input_words)
{
    struct list *output_words = list_expend_to_list(input_words);
    return put_list_into_arr(output_words);
}
