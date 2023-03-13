#ifndef EXPANSION_SUBSTITUTION_H
#define EXPANSION_SUBSTITUTION_H

#include <stdbool.h>

struct list *expansion_substitution(char *input, size_t *i,
                                    bool need_field_splitting);
bool beginning_substitution(char c);

#endif /* !EXPANSION_SUBSTITUTION_H */
