#ifndef EXPANSION_CMD_SUBST_H
#define EXPANSION_CMD_SUBST_H

#include "list.h"

struct list *expansion_dollar_par(char *input, size_t *i);
struct list *expansion_backquote(char *input, size_t *i);
bool beginning_substitution(char c);

#endif /* !EXPANSION_CMD_SUBST_H */
