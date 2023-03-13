#ifndef DICO_FUNC_H
#define DICO_FUNC_H

#include "ast_struct.h"

struct dico_func
{
    struct hash_map *hm;
};

struct dico_func *dico_func_init(void);
void dico_func_insert(struct dico_func *dv, char *key, struct ast *value);
struct ast *dico_func_get(struct dico_func *dv, const char *key);
void dico_func_remove(struct dico_func *dv, const char *key);
bool dico_func_in(struct dico_func *df, const char *name);
void dico_func_print(struct dico_func *dv);
void dico_func_destroy(struct dico_func *dv);

#endif /* !DICO_FUNC_H */
