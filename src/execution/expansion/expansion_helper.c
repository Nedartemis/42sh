#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "list_helper.h"
#include "ope_string.h"
#include "vector.h"

// vector = "vector->data::words1::"separator"::word2::"separator"::..."
// struct list *words is destroy
// ex  : vector = "begin:" ; words = ["hello", "world"] ; sep = ' '
// res : vector = "begin:hello world"
void concat_list_string_to_vector(struct vector *vector, struct list *words,
                                  char separator)
{
    if (list_empty(words))
    {
        list_destroy(words, destroy_string);
        return;
    }

    char *first = (char *)list_pop_front(words);
    vector_concat_string(vector, first);
    free(first);

    while (!list_empty(words))
    {
        // append the separator
        vector_append(vector, separator);
        // concat the word and free this word
        char *word = (char *)list_pop_front(words);
        vector_concat_string(vector, word);
        free(word);
    }

    list_destroy(words, destroy_string);
}

// simalar as concat_list_string_to_vector without the vector
// str : words1::"separator"::word2::"separator"::...
// struct list *words is destroy
// ex  : words = ["hello", "world"] ; sep = ' '
// res : str = "hello world"
char *concat_list_string(struct list *words, char separator)
{
    struct vector *vector = vector_init(8);
    concat_list_string_to_vector(vector, words, separator);
    char *str = vector_to_string(vector);
    vector_destroy(vector);
    return str;
}

// result [ vector::word1, word2, word3, ... ]
// clear the vector
// ex  : vector = "begin:" ; words = [ "hello", "world" ]
// res : words = [ "begin:hello", "world" ] ; vector = ""
void concat_vector_into_fist_word(struct vector *vector, struct list *words)
{
    if (vector->size == 0)
        return;

    if (list_size(words) == 0)
    {
        list_add(words, strndup(vector->data, vector->size));
        vector->size = 0;
        return;
    }

    // get first element
    char *first = list_pop_front(words);

    // concat
    vector_append(vector, 0);
    char *new_first = my_strcat(vector->data, first);

    // clear vector and free old first
    vector->size = 0;
    free(first);

    // put the new_word in the words
    list_prepend(words, new_first);
}

// result [ word1, word2, ..., wordn::vector ]
// clear the vector
// ex  : vector = ":end" ; words = [ "hello", "world" ]
// res : words = [ "hello", "world:end" ] ; vector = ""
void concat_vector_to_last_element(struct list *words, struct vector *vector)
{
    char *vector_to_str = vector_to_string(vector);

    if (list_size(words) == 0)
    {
        list_add(words, vector_to_str);
        vector->size = 0;
        return;
    }

    // get the last (remove it)
    char *last = list_pop_back(words);

    // concat last::vector
    vector_append(vector, 0);
    char *new_last = my_strcat(last, vector_to_str);
    free(vector_to_str);

    // clear vector and free last
    vector->size = 0;
    free(last);

    // put the new_word in the words
    list_add(words, new_last);
}

// result [ l1w1, l1w2, ... , l1wn::l2w1, l2w2, l2w2, ..., l2wn ]
// destroy l2
// ex  : l1 = [ "hello", "world" ] ; l2 = [ "toto", "tutu" ]
// res : l1 = [ "hello", "worldtoto", "tutu" ]
void concat_special_list(struct list *l1, struct list *l2)
{
    if (!list_empty(l1) && !list_empty(l2))
    {
        // get the last and first (remove it)
        char *last = list_pop_back(l1);
        char *first = list_pop_front(l2);

        // concat last::first
        char *new_last = my_strcat(last, first);

        // clear vector and free last
        free(last);
        free(first);

        // put the new_word in the words
        list_add(l1, new_last);
    }

    // concat the list + destroy l2
    list_concat(l1, l2);
}
