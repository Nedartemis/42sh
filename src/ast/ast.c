#include "ast.h"

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast2.h"
#include "ast_destroy.h"
#include "ast_destroy2.h"
#include "ast_graphviz.h"
#include "ast_pretty_printer.h"
#include "ast_pretty_printer2.h"
#include "ast_struct.h"
#include "list.h"
#include "run_ast.h"

void ast_init(struct ast *ast, enum AstType type,
              const struct ast_vtable *vtable)
{
    ast->type = type;
    ast->vtable = vtable;
}

struct ast *ast_list_init(struct list *asts)
{
    struct ast_list *ast_list = calloc(1, sizeof(struct ast_list));
    if (!ast_list)
        errx(4, "ast_list_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = { .run = run_list,
                                                  .destroy = ast_list_destroy,
                                                  .pretty_print =
                                                      ast_list_pretty_print,
                                                  .graph_viz =
                                                      ast_list_write_graph };

    // init ast (base)
    ast_init(&ast_list->base, AST_LIST, &ast_vtable);

    // init asts
    ast_list->asts = asts;

    // cast + return
    return &ast_list->base;
}

struct ast *ast_if_init(struct ast *conditions, struct ast *if_body,
                        struct ast *else_body)
{
    struct ast_if *ast_if = calloc(1, sizeof(struct ast_if));
    if (!ast_if)
        errx(4, "ast_if_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = { .run = run_if,
                                                  .destroy = ast_if_destroy,
                                                  .pretty_print =
                                                      ast_if_pretty_print,
                                                  .graph_viz =
                                                      ast_if_write_graph };

    // init ast (base)
    ast_init(&ast_if->base, AST_IF, &ast_vtable);

    // init conditions, if_body, else_body
    ast_if->conditions = conditions;
    ast_if->if_body = if_body;
    ast_if->else_body = else_body;

    // cast + return
    return &ast_if->base;
}

struct ast *ast_cmd_init(struct list *words, struct list *ast_prefixes)
{
    struct ast_cmd *ast_cmd = calloc(1, sizeof(struct ast_cmd));
    if (!ast_cmd)
        errx(4, "ast_cmd_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = { .run = run_cmd,
                                                  .destroy = ast_cmd_destroy,
                                                  .pretty_print =
                                                      ast_cmd_pretty_print,
                                                  .graph_viz =
                                                      ast_cmd_write_graph };

    // init ast (base)
    ast_init(&ast_cmd->base, AST_CMD, &ast_vtable);

    // init attributes
    ast_cmd->words = words;
    ast_cmd->ast_prefixes = ast_prefixes;

    // cast + return
    return &ast_cmd->base;
}

struct ast *ast_element_init(char *word)
{
    struct ast_element *ast_element = calloc(1, sizeof(struct ast_element));
    if (!ast_element)
        errx(4, "ast_element_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = {
        .run = NULL,
        .destroy = ast_element_destroy,
        .pretty_print = element_pretty_print,
    };

    // init ast (base)
    ast_init(&ast_element->base, AST_ELEMENT, &ast_vtable);

    // init word
    ast_element->word = word;

    // cast + return
    return &ast_element->base;
}

struct ast *ast_redir_init(enum TokenType redir_type, char *word)
{
    struct ast_redir *ast_redir = calloc(1, sizeof(struct ast_redir));
    if (!ast_redir)
        errx(4, "ast_redir_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = {
        .run = run_redir,
        .destroy = ast_redir_destroy,
        .pretty_print = ast_redir_pretty_print,
        //.graph_viz = ast_redir_write_graph
    };

    // init ast (base)
    ast_init(&ast_redir->base, AST_REDIR, &ast_vtable);

    // init arguments
    ast_redir->redir_type = redir_type;
    ast_redir->word = word;
    ast_redir->cmd = NULL;
    ast_redir->io_default = true;

    // cast + return
    return &ast_redir->base;
}

struct ast *ast_pipeline_init(struct ast *left, struct ast *right)
{
    struct ast_pipeline *ast_pipeline = calloc(1, sizeof(struct ast_pipeline));
    if (!ast_pipeline)
        errx(4, "ast_pipeline_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = {
        .run = run_pipeline,
        .destroy = ast_pipeline_destroy,
        .pretty_print = ast_pipeline_pretty_print,
        .graph_viz = ast_pipeline_write_graph
    };

    // init ast (base)
    ast_init(&ast_pipeline->base, AST_PIPELINE, &ast_vtable);

    // init left and right
    ast_pipeline->left = left;
    ast_pipeline->right = right;

    // init istrue
    ast_pipeline->istrue = true;

    // cast + return
    return &ast_pipeline->base;
}

struct ast *ast_and_or_init(enum TokenType type, struct ast *left,
                            struct ast *right)
{
    struct ast_and_or *ast_and_or = calloc(1, sizeof(struct ast_and_or));
    if (!ast_and_or)
        errx(4, "ast_and_or_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = {
        .run = run_and_or,
        .destroy = ast_and_or_destroy,
        .pretty_print = ast_and_or_pretty_print,
        .graph_viz = NULL // ast_and_or_write_graph
    };

    // init ast (base)
    ast_init(&ast_and_or->base, AST_AND_OR, &ast_vtable);

    // init type
    ast_and_or->type = type;

    // init left and right
    ast_and_or->left = left;
    ast_and_or->right = right;

    // cast + return
    return &ast_and_or->base;
}

struct ast *ast_while_init(enum TokenType type, struct ast *conditions,
                           struct ast *body)
{
    struct ast_while *ast_while = calloc(1, sizeof(struct ast_while));
    if (!ast_while)
        errx(4, "ast_while_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = {
        .run = run_while,
        .destroy = ast_while_destroy,
        .pretty_print = ast_while_pretty_print,
        .graph_viz = NULL // ast_while_write_graph
    };

    // init ast (base)
    ast_init(&ast_while->base, AST_WHILE, &ast_vtable);

    // init type, conditions, body
    ast_while->type = type;
    ast_while->conditions = conditions;
    ast_while->body = body;

    // cast + return
    return &ast_while->base;
}

struct ast *ast_for_init(char *var, struct list *words, struct ast *body)
{
    struct ast_for *ast_for = calloc(1, sizeof(struct ast_for));
    if (!ast_for)
        errx(4, "ast_for_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = {
        .run = run_for,
        .destroy = ast_for_destroy,
        .pretty_print = ast_for_pretty_print,
        .graph_viz = NULL // ast_for_write_graph
    };

    // init ast (base)
    ast_init(&ast_for->base, AST_FOR, &ast_vtable);

    // init var, words and body
    ast_for->var = var;
    ast_for->words = words;
    ast_for->body = body;

    // cast + return
    return &ast_for->base;
}
