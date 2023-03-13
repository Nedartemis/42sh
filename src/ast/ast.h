#ifndef AST_H
#define AST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast_struct.h"
#include "lexer.h"

// ast_init

void ast_init(struct ast *ast, enum AstType type,
              const struct ast_vtable *vtable);

struct ast *ast_list_init(struct list *asts);
struct ast *ast_if_init(struct ast *conditions, struct ast *if_body,
                        struct ast *else_body);
struct ast *ast_cmd_init(struct list *words, struct list *ast_prefixes);
struct ast *ast_element_init(char *word);
struct ast *ast_redir_init(enum TokenType redir_type, char *word);
struct ast *ast_pipeline_init(struct ast *left, struct ast *right);
struct ast *ast_and_or_init(enum TokenType type, struct ast *left,
                            struct ast *right);
struct ast *ast_while_init(enum TokenType type, struct ast *conditions,
                           struct ast *body);
struct ast *ast_for_init(char *var, struct list *words, struct ast *body);

#endif /* !AST_H */
