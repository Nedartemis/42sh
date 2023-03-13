#ifndef SUBSHELL_H
#define SUBSHELL_H

#include <sys/types.h>
#include <unistd.h>

#include "ast_struct.h"

pid_t fork_subshell(struct ast *ast_list);
int execute_subshell(struct ast *ast_list);

#endif /* !SUBSHELL_H */
