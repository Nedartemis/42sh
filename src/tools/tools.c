#include <stdio.h>
#include <stdlib.h>

#include "list.h"

// translate 'list' to a NULL TERMINATED string array
char **list_to_nt_array(struct list *list, int index)
{
    size_t size = list_size(list);
    char **nt_array = calloc(size + !index, sizeof(char *));

    for (size_t i = index; i < size; i++)
    {
        char *element = (char *)list_get(list, i);
        nt_array[i + !index - 1] = element;
    }

    return nt_array;
}

struct list *arr_to_list(char **arr, size_t size)
{
    struct list *l = list_init();
    for (size_t i = 0; i < size; i++)
        list_add(l, (void *)arr[i]);
    return l;
}
