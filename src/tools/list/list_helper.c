#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "list.h"

void list_concat(struct list *l1, struct list *l2)
{
    while (!list_empty(l2))
    {
        void *elem = list_pop_front(l2);
        list_add(l1, elem);
    }
    free(l2);
}

void *list_pop_back(struct list *list)
{
    if (!list->next)
        errx(3, "list_pop_front : try to pop an empty list\n");

    struct list *current = list;
    for (; current->next->next; current = current->next)
        continue;

    // get data
    struct list *last = current->next;
    void *data = last->data;

    // remove last from list
    free(last);
    current->next = NULL;

    return data;
}

void list_change_value(struct list *list, size_t i, char *new_data,
                       void (*destroy_data)(void *))
{
    struct list *current = list->next;
    for (size_t j = 0; current && j < i; j++, current = current->next)
    {
        continue;
    }

    if (!current)
        errx(3, "list_change_value : index out of range (i = %ld)", i);

    // destroy old data
    destroy_data(current->data);

    // put the new one
    current->data = new_data;
}
