#ifndef OPE_STRING_H
#define OPE_STRING_H

#include <stdbool.h>

bool is_number(const char *str);
bool valid_begin_var(char c);
bool is_char_name_var(char c);
bool is_valid_name_var(const char *name, size_t i);
void print_string(void *data);
void destroy_string(void *data);
char *my_strcat(char *str1, char *str2);

#endif /* !OPE_STRING_H */
