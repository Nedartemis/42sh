#include "vector.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// return a new instance of vector with a capacity of n
// or NULL if an error occurs
struct vector *vector_init(size_t n)
{
    struct vector *v = calloc(1, sizeof(struct vector));
    if (!v)
    {
        return NULL;
    }
    char *d = calloc(n, sizeof(int));
    v->size = 0;
    v->data = d;
    v->capacity = n;
    return v;
}

// destroy the instance v and free the used memory
void vector_destroy(struct vector *v)
{
    free(v->data);
    v->size = 0;
    v->capacity = 0;
    free(v);
}

void vector_print(struct vector *v)
{
    printf("%s", v->data);
}

// resize v's capacity so it can contain n elements
// if n < v->size, its content is cropped
// returns the resized vector
struct vector *vector_resize(struct vector *v, size_t n)
{
    if (!v)
    {
        return NULL;
    }
    if (n == v->capacity)
    {
        return v;
    }
    else if (n > v->capacity)
    {
        v->data = realloc(v->data, (sizeof(char) * n));
        if (!v)
        {
            return NULL;
        }
        v->capacity = n;
        return v;
    }
    else if (n < v->capacity)
    {
        v->data = realloc(v->data, (sizeof(char) * n));
        if (!v)
        {
            return NULL;
        }
        v->capacity = n;
        v->size = n;
        return v;
    }
    return v;
}

// append elt to the end of the vector v, resizing if necessary
// returns NULL if any error occurred
struct vector *vector_append(struct vector *v, char elt)
{
    if (!v)
    {
        return NULL;
    }
    if ((v->size + 1) > v->capacity)
    {
        v = vector_resize(v, v->capacity * 2);
    }

    v->data[v->size++] = elt;
    return v;
}

// remove every elements from v, leaving its size to 0
// and returning a fresh vector of capacity n, NULL if any error occurred
struct vector *vector_reset(struct vector *v, size_t n)
{
    v->size = 0;
    v = vector_resize(v, n);
    v->size = 0;
    return v;
}

// remove the i nth element from the vector v
// returns the new vector, NULL if any error occurred
struct vector *vector_remove(struct vector *v, size_t i)
{
    if (!v)
    {
        return NULL;
    }
    if (i >= v->size)
    {
        return NULL;
    }
    if (v->size - 1 < (v->capacity / 2))
    {
        v = vector_resize(v, (v->capacity / 2));
    }
    for (size_t j = i; j < v->size - 1; j++)
    {
        v->data[j] = v->data[j + 1];
    }
    v->size--;
    return v;
}

void vector_concat_string(struct vector *vector, char *str)
{
    for (int i = 0; str[i]; i++)
    {
        vector_append(vector, str[i]);
    }
}

char *vector_to_string(struct vector *vector)
{
    return strndup(vector->data, vector->size);
}

char *vector_to_string_begin(struct vector *vector, size_t begin, size_t size)
{
    if (size > vector->size - begin)
        errx(9,
             "vector_to_string : size too high ; vector->size = %ld ; begin = "
             "%ld ; size = %ld",
             vector->size, begin, size);

    return strndup(vector->data + begin, size);
}
