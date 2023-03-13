#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "log.h"
#include "null_array.h"
#include "ope_string.h"
#include "sh_instance.h"
#include "vector.h"

#define VECTOR_SIZE 16

struct ifs_all_var
{
    char ifs_white_space[4];
    char *ifs_var;
};

// return char *ifs_var
void get_ifs_var(struct ifs_all_var *iav)
{
    char *ifs = getenv("IFS");
    if (!ifs)
    {
        iav->ifs_var = NULL;
        return;
    }

    struct vector *ifs_var = vector_init(VECTOR_SIZE);
    int j = 0;
    iav->ifs_white_space[0] = '\0';

    for (int i = 0; ifs[i]; i++)
    {
        if (array_char_in(" \t\n", ifs[i]))
        {
            if (!array_char_in(iav->ifs_white_space, ifs[i]))
            {
                iav->ifs_white_space[j++] = ifs[i];
                iav->ifs_white_space[j] = '\0';
            }
        }

        vector_append(ifs_var, ifs[i]);
    }

    char *str = vector_to_string(ifs_var);
    vector_destroy(ifs_var);
    iav->ifs_var = str;
}

static void pass_ifs_white_space(char *word, int *i, struct ifs_all_var *iav)
{
    for (; word[*i] && array_char_in(iav->ifs_white_space, word[*i]); (*i)++)
        continue;
}

static void delimit(struct list *new_list, char *word, int *i,
                    struct ifs_all_var *iav)
{
    for (; word[*i];)
    {
        // delimit the word
        struct vector *v = vector_init(VECTOR_SIZE);
        for (; word[*i] && !array_char_in(iav->ifs_var, word[*i]); (*i)++)
        {
            vector_append(v, word[*i]);
        }
        // add the word to the list (can be empty)
        list_add(new_list, vector_to_string(v));
        vector_destroy(v);

        pass_ifs_white_space(word, i, iav);

        if (word[*i] && array_char_in(iav->ifs_var, word[*i]))
            (*i)++;

        pass_ifs_white_space(word, i, iav);
    }
}

static void copy_without_ending(struct list *new_list, char *word, int *i,
                                struct ifs_all_var *iav)
{
    int j = strlen(word);
    for (; word[j - 1] && array_char_in(iav->ifs_white_space, word[j - 1]); j--)
        continue;
    list_add(new_list, strndup(word + *i, j - *i));
}

void field_splitting(struct list **words)
{
    // get ifs var
    struct ifs_all_var iav;
    get_ifs_var(&iav);

    struct list *new_list = list_init();
    struct sh *sh = sh_instance_get();
    if (!iav.ifs_var) // case sh = NULL (unset)
    {
        iav.ifs_white_space[0] = ' ';
        iav.ifs_white_space[1] = '\t';
        iav.ifs_white_space[2] = '\n';
        iav.ifs_white_space[3] = 0;
    }
    else
        log_mess(sh, "if_var = '%s' ; ifs_white_space = '%s'\n", iav.ifs_var,
                 iav.ifs_white_space);

    // treat each word in the list *words
    while (!list_empty(*words))
    {
        char *word = (char *)list_pop_front(*words);
        log_mess(sh, "word = '%s'; sh->log=%d\n", word, sh->log);
        if (!word[0])
        {
            free(word);
            continue;
        }

        int i = 0;

        // remove beginning ifs white spaces
        pass_ifs_white_space(word, &i, &iav);

        if (iav.ifs_var) // 2 : if IFS is null, no filed splitting shall be
                         // performed
            delimit(new_list, word, &i, &iav);
        else
            copy_without_ending(new_list, word, &i, &iav);

        free(word);
    }

    if (sh->log)
        list_print(new_list, print_string);
    log_mess(sh, "\n");

    // destroy
    list_destroy(*words, destroy_string);
    free(iav.ifs_var);

    // return value
    *words = new_list;
}
