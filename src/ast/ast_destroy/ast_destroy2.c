#include "ast_destroy2.h"

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "ast_destroy.h"
#include "ast_struct.h"
#include "list.h"

void ast_while_destroy(struct ast *ast)
{
    struct ast_while *ast_while = (struct ast_while *)ast;
    ast_destroy(ast_while->conditions);
    ast_destroy(ast_while->body);
    free(ast_while);
}

void ast_for_destroy(struct ast *ast)
{
    struct ast_for *ast_for = (struct ast_for *)ast;
    ast_destroy(ast_for->body);
    list_destroy(ast_for->words, destroy_word_cmd);
    destroy_word_cmd(ast_for->var);
    free(ast_for);
}

void ast_prefix_destroy(struct ast *ast)
{
    struct ast_prefix *ast_prefix = (struct ast_prefix *)ast;
    free(ast_prefix->ass_word);
    free(ast_prefix);
}

void ast_func_destroy(struct ast *ast)
{
    struct ast_func *ast_func = (struct ast_func *)ast;
    free(ast_func->name);
    ast_destroy(ast_func->body);
    free(ast_func);
}

void ast_subshell_destroy(struct ast *ast)
{
    struct ast_subshell *ast_subshell = (struct ast_subshell *)ast;
    ast_destroy(ast_subshell->ast_list);
    free(ast_subshell);
}

void ast_case_destroy(struct ast *ast)
{
    struct ast_case *ast_case = (struct ast_case *)ast;
    free(ast_case->word);
    list_destroy(ast_case->items, destroy_ast_void);
    free(ast_case);
}

void ast_item_destroy(struct ast *ast)
{
    struct ast_item *ast_item = (struct ast_item *)ast;
    list_destroy(ast_item->words, destroy_word_cmd);
    ast_destroy(ast_item->body);
    free(ast_item);
}
