#include "dico_word.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hash_map.h"
#include "list.h"
#include "tools.h"

static void destroy_value(void *value)
{
    free((char *)value);
}

struct dico_word *dico_word_init(void)
{
    struct dico_word *dv = calloc(1, sizeof(struct dico_word *));
    if (!dv)
        errx(3, "dico_word_init : not enough memory\n");

    dv->hm = hash_map_init(10, destroy_value);

    return dv;
}

void dico_word_insert(struct dico_word *dv, const char *name, const char *value)
{
    hash_map_insert(dv->hm, name, (void *)strdup(value));
}

const char *dico_word_get(struct dico_word *dv, const char *name)
{
    return (char *)hash_map_get(dv->hm, name);
}

bool dico_word_in(struct dico_word *dv, const char *name)
{
    return dico_word_get(dv, name) != NULL;
}

void dico_word_remove(struct dico_word *dv, const char *name)
{
    hash_map_remove(dv->hm, name);
    unsetenv(name);
}

static void print_value(void *data)
{
    printf("%s", (char *)data);
}

void dico_word_print(struct dico_word *dv)
{
    hash_map_print(dv->hm, print_value);
}

void dico_word_destroy(struct dico_word *dv)
{
    hash_map_destroy(dv->hm);
    free(dv);
}
