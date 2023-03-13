#include "ast2.h"

#include <err.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "ast_destroy.h"
#include "ast_destroy2.h"
#include "ast_graphviz.h"
#include "ast_pretty_printer.h"
#include "ast_pretty_printer2.h"
#include "ast_struct.h"
#include "run_ast.h"
#include "run_ast2.h"

struct ast *ast_prefix_init(char *ass_word)
{
    struct ast_prefix *ast_prefix = calloc(1, sizeof(struct ast_prefix));
    if (!ast_prefix)
        errx(4, "ast_prefix_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = {
        .run = run_prefix,
        .destroy = ast_prefix_destroy,
        .pretty_print = ast_prefix_pretty_print,
        .graph_viz = NULL // ast_prefix_write_graph
    };

    // init ast (base)
    ast_init(&ast_prefix->base, AST_PREFIX, &ast_vtable);

    // init ass_word
    ast_prefix->ass_word = ass_word;

    // cast + return
    return &ast_prefix->base;
}

struct ast *ast_func_init(char *name, struct ast *body)
{
    struct ast_func *ast_func = calloc(1, sizeof(struct ast_func));
    if (!ast_func)
        errx(4, "ast_func_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = {
        .run = run_func,
        .destroy = ast_func_destroy,
        .pretty_print = ast_func_pretty_print,
        .graph_viz = NULL // ast_func_write_graph
    };

    // init ast (base)
    ast_init(&ast_func->base, AST_FUNC, &ast_vtable);

    // init name and body
    ast_func->name = name;
    ast_func->body = body;
    ast_func->is_assign = false;

    // cast + return
    return &ast_func->base;
}

struct ast *ast_subshell_init(struct ast *ast_list)
{
    struct ast_subshell *ast_subshell = calloc(1, sizeof(struct ast_subshell));
    if (!ast_subshell)
        errx(4, "ast_subshell_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = {
        .run = run_subshell,
        .destroy = ast_subshell_destroy,
        .pretty_print = ast_subshell_pretty_print,
        .graph_viz = NULL // ast_subshell_write_graph
    };

    // init ast (base)
    ast_init(&ast_subshell->base, AST_SUBSHELL, &ast_vtable);

    // init ast_list
    ast_subshell->ast_list = ast_list;

    // cast + return
    return &ast_subshell->base;
}

struct ast *ast_case_init(char *word, struct list *items)
{
    struct ast_case *ast_case = calloc(1, sizeof(struct ast_case));
    if (!ast_case)
        errx(4, "ast_case_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = {
        .run = run_case,
        .destroy = ast_case_destroy,
        .pretty_print = ast_case_pretty_print,
        .graph_viz = NULL // ast_case_write_graph
    };

    // init ast (base)
    ast_init(&ast_case->base, AST_CASE, &ast_vtable);

    // init word and list
    ast_case->word = word;
    ast_case->items = items;

    // cast + return
    return &ast_case->base;
}

struct ast *ast_item_init(struct list *words, struct ast *body)
{
    struct ast_item *ast_item = calloc(1, sizeof(struct ast_item));
    if (!ast_item)
        errx(4, "ast_item_init : Not enough memory");

    // init vtable
    static const struct ast_vtable ast_vtable = {
        .run = NULL, // run_item,
        .destroy = ast_item_destroy,
        .pretty_print = ast_item_pretty_print,
        .graph_viz = NULL // ast_item_write_graph
    };

    // init ast (base)
    ast_init(&ast_item->base, AST_ITEM, &ast_vtable);

    // init words and body
    ast_item->words = words;
    ast_item->body = body;

    // cast + return
    return &ast_item->base;
}
