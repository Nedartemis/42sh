#ifndef LIST_H
#define LIST_H

#include <stdbool.h>
#include <stddef.h>

struct list
{
    void *data;
    struct list *next;
};

struct list *list_init(void);
size_t list_size(struct list *list);
void list_add(struct list *list, void *data);
void list_prepend(struct list *list, void *data);
bool list_empty(struct list *list);
void *list_first(struct list *list);
void *list_pop_front(struct list *list);
void *list_get(struct list *list, size_t index);
void list_print(struct list *list, void(print_data)(void *));
void list_destroy(struct list *list, void(destroy_data)(void *));

#endif /* !LIST_H */
