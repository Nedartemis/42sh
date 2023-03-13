#include "list.h"

#include <err.h>
#include <stdio.h>
#include <stdlib.h>

struct list *list_init(void)
{
    struct list *res = calloc(1, sizeof(struct list));
    if (!res)
        errx(3, "list_init: not enough memory");
    return res;
}

size_t list_size(struct list *list)
{
    struct list *temp = list->next;
    size_t size = 0;

    for (; temp; temp = temp->next, size++)
        continue;

    return size;
}

void list_add(struct list *list, void *data)
{
    // init next
    struct list *next = list_init();
    next->data = data;

    // find end
    struct list *temp = list;
    for (; temp->next; temp = temp->next)
        continue;

    // link
    temp->next = next;
}

void list_prepend(struct list *list, void *data)
{
    // init new list
    struct list *new = list_init();
    new->data = data;

    // link sentinel->new->first
    struct list *first = list->next;
    list->next = new;
    new->next = first;
}

bool list_empty(struct list *list)
{
    return (!list->next);
}

void *list_first(struct list *list)
{
    if (list_empty(list))
        errx(3, "list_first: first doesn't exist");
    return list->next->data;
}

void *list_pop_front(struct list *list)
{
    if (list_empty(list))
        errx(3, "list_pop_front: first doesn't exist");
    struct list *front = list->next;

    // link
    list->next = front->next;

    void *data = front->data;
    free(front);
    return data;
}

void *list_get(struct list *list, size_t index)
{
    struct list *temp = list->next;
    for (size_t i = 0; i < index && temp; temp = temp->next, i++)
        continue;
    if (!temp)
        errx(3, "list_get: index out of range (index = %ld)", index);
    return temp->data;
}

void list_print(struct list *list, void(print_data)(void *))
{
    struct list *temp = list->next;
    printf("[");
    for (; temp; temp = temp->next)
    {
        print_data(temp->data);
        if (temp->next)
            printf(", ");
    }
    printf("]");
}

void list_destroy(struct list *list, void(destroy_data)(void *))
{
    if (!list)
        return;
    struct list *temp = list->next;
    for (; temp;)
    {
        // printf("free\n");
        destroy_data(temp->data);
        struct list *next = temp->next;
        free(temp);
        temp = next;
    }

    // free sentinel
    free(list);
}
