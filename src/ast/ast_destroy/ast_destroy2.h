#ifndef AST_DESTROY2_H
#define AST_DESTROY2_H

#include "ast.h"
#include "ast_struct.h"
#include "list.h"

void ast_while_destroy(struct ast *ast);
void ast_for_destroy(struct ast *ast);
void ast_prefix_destroy(struct ast *ast);
void ast_func_destroy(struct ast *ast);
void ast_subshell_destroy(struct ast *ast);
void ast_case_destroy(struct ast *ast);
void ast_item_destroy(struct ast *ast);

#endif /* !AST_DESTROY22_H */
