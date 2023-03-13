#ifndef LIST_HELPER_H
#define LIST_HELPER_H

#include "list.h"

void list_concat(struct list *l1, struct list *l2);
void *list_pop_back(struct list *list);

void list_change_value(struct list *list, size_t i, char *new_data,
                       void (*destroy_data)(void *));

#endif /* !LIST_HELPER_H */
