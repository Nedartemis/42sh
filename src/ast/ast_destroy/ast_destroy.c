#include "ast_destroy.h"

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"
#include "ast_struct.h"
#include "list.h"

// use to destroy asts
void destroy_ast_void(void *data)
{
    struct ast *ast = (struct ast *)data;
    ast_destroy(ast);
}

// use to destroy word in cmd function
void destroy_word_cmd(void *data)
{
    char *word = (char *)data;
    free(word);
}

// ---------------------------------------------------

void ast_destroy(struct ast *ast)
{
    if (ast)
        ast->vtable->destroy(ast);
}

void ast_cmd_destroy(struct ast *ast)
{
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;
    list_destroy(ast_cmd->words, destroy_word_cmd);
    list_destroy(ast_cmd->ast_prefixes, destroy_ast_void);
    free(ast_cmd);
}

void ast_if_destroy(struct ast *ast)
{
    struct ast_if *ast_if = (struct ast_if *)ast;
    ast_destroy(ast_if->conditions);
    ast_destroy(ast_if->if_body);
    ast_destroy(ast_if->else_body);
    free(ast_if);
}

void ast_list_destroy(struct ast *ast)
{
    struct ast_list *ast_list = (struct ast_list *)ast;
    list_destroy(ast_list->asts, destroy_ast_void);
    free(ast_list);
}

void ast_element_destroy(struct ast *ast)
{
    struct ast_element *ast_element = (struct ast_element *)ast;
    free(ast_element->word);
    free(ast_element);
}

void ast_redir_destroy(struct ast *ast)
{
    struct ast_redir *ast_redir = (struct ast_redir *)ast;

    // free attributs
    ast_destroy(ast_redir->cmd);
    free(ast_redir->word);
    // free itself
    free(ast_redir);
}

void ast_pipeline_destroy(struct ast *ast)
{
    struct ast_pipeline *ast_pipeline = (struct ast_pipeline *)ast;

    struct ast *left = ast_pipeline->left;
    struct ast *right = ast_pipeline->right;

    if (ast_pipeline->left)
        left->vtable->destroy(left);

    if (ast_pipeline->right)
        right->vtable->destroy(right);

    free(ast_pipeline);
}

void ast_and_or_destroy(struct ast *ast)
{
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast;

    struct ast *left = ast_and_or->left;
    struct ast *right = ast_and_or->right;

    if (ast_and_or->left)
        left->vtable->destroy(left);

    if (ast_and_or->right)
        right->vtable->destroy(right);

    free(ast_and_or);
}
