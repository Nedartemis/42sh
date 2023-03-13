#ifndef STRUCT_AST_H
#define STRUCT_AST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"

/* STRUCT FOR GRAPHVIZ */
struct node_counters
{
    size_t nc_pipeline;
    size_t nc_words;
    size_t nc_cmd;
    size_t nc_if;
    size_t nc_list;
    size_t nc_prefix;
};
//-----------------------

enum AstType
{
    AST_LIST,
    AST_IF,
    AST_CMD,
    AST_ELEMENT,
    AST_REDIR,
    AST_PIPELINE,
    AST_AND_OR,
    AST_WHILE,
    AST_FOR,
    AST_PREFIX,
    AST_FUNC,
    AST_SUBSHELL,
    AST_CASE,
    AST_ITEM,
};

struct ast_vtable;

struct ast
{
    enum AstType type;
    const struct ast_vtable *vtable;
};

struct ast_vtable
{
    int (*run)(struct ast *ast);
    void (*pretty_print)(struct ast *ast);
    void (*graph_viz)(struct ast *ast, FILE *file, struct node_counters *nc);
    void (*destroy)(struct ast *ast);
};

struct ast_list
{
    struct ast base;
    struct list *asts;
};

struct ast_if
{
    struct ast base;
    struct ast *conditions; // list
    struct ast *if_body;
    struct ast *else_body;
};

struct ast_cmd
{
    struct ast base;
    struct list *words; // list of char*
    struct list *ast_prefixes; // list of asts (redir)
};

struct ast_pipeline
{
    struct ast base;
    struct ast *left; // ast_cmd
    struct ast *right; // pipeline
    bool istrue; // true by default
};

struct ast_element
{
    struct ast base;
    char *word;
};

struct ast_redir
{
    struct ast base;
    int io_number; // default: FILENO_STDOUT
    enum TokenType redir_type;
    char *word;
    struct ast *cmd;
    bool io_default;
};

struct ast_and_or
{
    struct ast base;
    enum TokenType type;
    struct ast *left;
    struct ast *right;
};

struct ast_while
{
    struct ast base;
    enum TokenType type;
    struct ast *conditions;
    struct ast *body;
};

struct ast_for
{
    struct ast base;
    char *var;
    struct list *words; // list of char*
    struct ast *body; // ast_list
};

struct ast_prefix
{
    struct ast base;
    char *ass_word;
};

struct ast_func
{
    struct ast base;
    char *name;
    struct ast *body; // shell_command
    bool is_assign;
};

struct ast_subshell
{
    struct ast base;
    struct ast *ast_list;
};

struct ast_case
{
    struct ast base;
    char *word;
    struct list *items;
};

struct ast_item
{
    struct ast base;
    struct list *words;
    struct ast *body;
};

#endif /* !STRUCT_AST_H */
