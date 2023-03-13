#ifndef AST2_H
#define AST2_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "ast_struct.h"
#include "lexer.h"

// ast_init

struct ast *ast_prefix_init(char *ass_word);
struct ast *ast_func_init(char *name, struct ast *body);
struct ast *ast_subshell_init(struct ast *ast_list);
struct ast *ast_case_init(char *word, struct list *items);
struct ast *ast_item_init(struct list *words, struct ast *body);

#endif /* !AST2_H */
