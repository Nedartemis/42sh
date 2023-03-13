#ifndef PRETTY_PRINTER2_H
#define PRETTY_PRINTER2_H

#include "ast.h"
#include "ast_struct.h"
#include "list.h"

void ast_redir_pretty_print(struct ast *ast);
void ast_and_or_pretty_print(struct ast *ast);
void ast_while_pretty_print(struct ast *ast);
void ast_for_pretty_print(struct ast *ast);
void ast_prefix_pretty_print(struct ast *ast);
void ast_func_pretty_print(struct ast *ast);
void ast_subshell_pretty_print(struct ast *ast);
void ast_case_pretty_print(struct ast *ast);
void ast_item_pretty_print(struct ast *ast);

#endif /* !PRETTY_PRINTER2_H */
