#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

// ======================== STRUCTS ========================== //

struct vector
{
    size_t size;
    size_t capacity;
    char *data;
};

// ======================== VECTORS ========================== //

// create a vector of size n
struct vector *vector_init(size_t n);
// free the vector v
void vector_destroy(struct vector *v);
// wrapper to printf to print the vector's data
void vector_print(struct vector *v);
// change v's size to n
struct vector *vector_resize(struct vector *v, size_t n);
// insert elt at the end of the vector
struct vector *vector_append(struct vector *v, char elt);
// resize v's capacity to n and remove its elements
struct vector *vector_reset(struct vector *v, size_t n);
// remove i nth element from v
struct vector *vector_remove(struct vector *v, size_t i);

void vector_concat_string(struct vector *vector, char *str);

char *vector_to_string(struct vector *vector);

char *vector_to_string_begin(struct vector *vector, size_t begin, size_t size);

#endif /* !VECTOR_H */
