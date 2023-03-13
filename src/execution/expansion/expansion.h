#ifndef EXPENSION_H
#define EXPENSION_H

#include "struct.h"
#include "vector.h"

int assign_word(const char *w);
struct list *expansion(char *input, bool need_field_splitting);
void treat_env_var(struct list *prefixes, int set_unset);

struct list *expansion_dollar(char *input, size_t *i);
struct list *list_expend_to_list(struct list *input_words);
char **list_expend_to_arr(struct list *input_words);

#endif /* !EXPENSION_H */
