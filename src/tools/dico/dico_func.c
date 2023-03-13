#include "dico_func.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast_destroy.h"
#include "ast_pretty_printer.h"
#include "ast_struct.h"
#include "hash_map.h"

static void destroy_value(void *value)
{
    struct ast *ast = (struct ast *)value;
    ast->vtable->destroy(ast);
    return;
}

struct dico_func *dico_func_init(void)
{
    struct dico_func *df = calloc(1, sizeof(struct dico_func));
    if (!df)
        errx(3, "dico_func_init : not enough memory\n");

    df->hm = hash_map_init(10, destroy_value);

    return df;
}

void dico_func_insert(struct dico_func *df, char *name, struct ast *value)
{
    hash_map_insert(df->hm, name, value);
}

struct ast *dico_func_get(struct dico_func *df, const char *name)
{
    return (struct ast *)hash_map_get(df->hm, name);
}

void dico_func_remove(struct dico_func *df, const char *name)
{
    hash_map_remove(df->hm, name);
}

bool dico_func_in(struct dico_func *df, const char *name)
{
    return dico_func_get(df, name) != NULL;
}

static void print_value(void *data)
{
    ast_pretty_print((struct ast *)data);
}

void dico_func_print(struct dico_func *df)
{
    hash_map_print(df->hm, print_value);
}

void dico_func_destroy(struct dico_func *df)
{
    hash_map_destroy(df->hm);
    free(df);
}
