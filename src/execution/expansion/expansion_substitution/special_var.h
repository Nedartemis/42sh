#ifndef SPECIAL_VAR_H
#define SPECIAL_VAR_H

#include "struct.h"
#include "vector.h"

bool is_env_var(const char *name);
bool is_special_var(const char *name);
bool is_special_char(const char c);
struct list *special_substitution(const char *name);

#endif /* !SPECIAL_VAR_H */
