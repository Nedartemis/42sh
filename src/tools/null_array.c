#include <stdbool.h>
#include <stdio.h>
#include <string.h>

bool array_char_in(const char arr[], char c)
{
    for (int i = 0; arr[i]; i++)
    {
        if (arr[i] == c)
            return true;
    }
    return false;
}

bool array_string_in(const char *arr[], const char *str)
{
    for (int i = 0; arr[i]; i++)
    {
        if (strcmp(arr[i], str) == 0)
        {
            return true;
        }
    }

    return false;
}
