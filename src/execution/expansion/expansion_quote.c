#include <assert.h>
#include <ctype.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "expansion.h"
#include "expansion_helper.h"
#include "expansion_substitution.h"
#include "expansion_var.h"
#include "null_array.h"
#include "vector.h"

void expansion_single_quote(char *word, size_t *i, struct vector *vector)
{
    assert(word[*i] == '\'');

    for (*i += 1; word[*i] && word[*i] != '\''; *i += 1)
    {
        vector_append(vector, word[*i]);
    }

    assert(word[*i] == '\'');
    *i += 1;
}

void expension_escape_char(char *word, size_t *i, struct vector *vector)
{
    assert(word[*i] == '\\');
    *i += 1;
    vector_append(vector, word[*i]);
    *i += 1;
}

void expension_double_quote(char *word, size_t *i, struct vector *vector)
{
    assert(word[*i] == '"');

    char char_escape_keep_meaning[] = {
        '$', '`', '"', '\\', '\n', '\0',
    };

    for (*i += 1; word[*i] && word[*i] != '"';)
    {
        char c = word[*i];
        if (c == '\\' && array_char_in(char_escape_keep_meaning, word[*i + 1]))
            expension_escape_char(word, i, vector);
        else if (beginning_substitution(c))
        {
            struct list *new_words = expansion_substitution(word, i, false);
            concat_list_string_to_vector(vector, new_words, ' ');
        }
        else
        {
            vector_append(vector, word[*i]);
            *i += 1;
        }
    }

    assert(word[*i] == '"');
    *i += 1;
}
