#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_number(const char *str)
{
    if (!str || !str[0])
        return false;

    for (int i = 0; str[i]; i++)
    {
        if (!isdigit(str[i]))
            return false;
    }
    return true;
}

bool valid_begin_var(char c)
{
    return c == '_' || isalpha(c);
}

bool is_char_name_var(char c)
{
    return c == '_' || isdigit(c) || isalpha(c);
}

bool is_valid_name_var(const char *name, size_t size)
{
    if (name[0] == 0)
        return false;

    if (isdigit(name[0]))
        return false;

    for (size_t i = 0; i < size; i++)
    {
        if (!is_char_name_var(name[i]))
            return false;
    }

    return true;
}

void destroy_string(void *data)
{
    char *str = (char *)data;
    free(str);
}

void print_string(void *data)
{
    printf("<%s>", (char *)data);
}

// str1::str2
char *my_strcat(char *str1, char *str2)
{
    // get sizes
    size_t size1 = strlen(str1);
    size_t size2 = strlen(str2);
    size_t new_size = size2 + size1 + 1;

    // realloc
    char *new_str = calloc(new_size, sizeof(char));

    // copy
    memcpy(new_str, str1, size1 + 1);

    // concat
    strncat(new_str, str2, size2);

    return new_str;
}
