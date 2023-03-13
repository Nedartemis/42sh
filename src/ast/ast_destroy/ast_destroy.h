#ifndef AST_DESTROY_H
#define AST_DESTROY_H

#include "ast.h"
#include "ast_struct.h"
#include "list.h"

// utilities
void destroy_word_cmd(void *data);

void destroy_ast_void(void *data);
void ast_destroy(struct ast *ast);
void ast_cmd_destroy(struct ast *ast);
void ast_if_destroy(struct ast *ast);
void ast_list_destroy(struct ast *ast);
void ast_element_destroy(struct ast *ast);
void ast_redir_destroy(struct ast *ast);
void ast_pipeline_destroy(struct ast *ast);
void ast_and_or_destroy(struct ast *ast);

#endif /* !AST_DESTROY_H */
