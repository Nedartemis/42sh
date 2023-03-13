#ifndef RUN_AST2_H
#define RUN_AST2_H

#include "ast_struct.h"

int run_func(struct ast *ast);
int run_subshell(struct ast *ast);
int run_case(struct ast *ast);

#endif /* !RUN_AST2_H */
