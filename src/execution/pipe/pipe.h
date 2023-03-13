#ifndef PIPE_H
#define PIPE_H

#include <sys/types.h>

#include "ast_struct.h"

int exec_pipe(struct ast *ast_left, struct ast *ast_right);

#endif /* !PIPE_H */
