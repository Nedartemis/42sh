#ifndef DICO_VAR_H
#define DICO_VAR_H

#include <stdbool.h>

struct dico_word
{
    struct hash_map *hm;
};

struct dico_word *dico_word_init(void);
void dico_word_insert(struct dico_word *dv, const char *key, const char *value);
const char *dico_word_get(struct dico_word *dv, const char *key);
bool dico_word_in(struct dico_word *dv, const char *name);
void dico_word_remove(struct dico_word *dv, const char *key);
void dico_word_print(struct dico_word *dv);
void dico_word_destroy(struct dico_word *dv);

#endif /* !DICO_VAR_H */
