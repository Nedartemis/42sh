#ifndef TOOLS_H
#define TOOLS_H

#include <stdlib.h>

#include "list.h"

// ========================  LISTS  ========================== //

char **list_to_nt_array(struct list *list, int index);
struct list *arr_to_list(char **arr, size_t size);

// ========================  REDIR  ========================== //

int pipe_redirect(char **left, char **right);

#endif /* !TOOLS_H */
