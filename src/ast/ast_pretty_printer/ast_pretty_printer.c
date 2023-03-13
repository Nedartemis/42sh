#include "ast_pretty_printer.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "ast_destroy.h"
#include "ast_struct.h"
#include "list.h"
#include "token.h"

static int nb_tabs = 0;

void pretty_print_tabs(void)
{
    for (int i = 0; i < nb_tabs; i++)
        printf(" ");
}

void change_tabs(int i)
{
    nb_tabs += i * 3;
}

void print_word(void *data)
{
    char *word = data;
    printf("\"%s\" ", word);
}

static void print_list_ast(struct list *asts)
{
    struct list *temp = asts->next;
    for (; temp; temp = temp->next)
    {
        ast_pretty_print((struct ast *)temp->data);
        if (temp->next)
            printf("; ");
    }
}

static void print_list_string(struct list *words)
{
    struct list *temp = words->next;
    for (; temp; temp = temp->next)
    {
        printf("%s", (char *)temp->data);
        if (temp->next)
            printf(" ");
    }
}

// use to print asts
static void print_ast(void *data)
{
    struct ast *ast = (struct ast *)data;
    ast_pretty_print(ast);
}

void ast_pretty_print(struct ast *ast)
{
    if (!ast)
        return;
    pretty_print_tabs();
    ast->vtable->pretty_print(ast);
}

void ast_cmd_pretty_print(struct ast *ast)
{
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;

    // prefixes
    if (!list_empty(ast_cmd->ast_prefixes))
        list_print(ast_cmd->ast_prefixes, print_ast);
    // words
    print_list_string(ast_cmd->words);
}

void ast_if_pretty_print(struct ast *ast)
{
    struct ast_if *ast_if = (struct ast_if *)ast;
    printf("if ");
    ast_pretty_print(ast_if->conditions);
    printf("; then \n");
    change_tabs(1);
    ast_pretty_print(ast_if->if_body);
    printf(";\n");
    change_tabs(-1);
    if (ast_if->else_body)
    {
        pretty_print_tabs();
        printf("else\n");
        change_tabs(1);
        ast_pretty_print(ast_if->else_body);
        printf(";\n");
        change_tabs(-1);
    }
    printf("fi\n");
}

void ast_list_pretty_print(struct ast *ast)
{
    struct ast_list *ast_list = (struct ast_list *)ast;
    print_list_ast(ast_list->asts);
}

void element_pretty_print(struct ast *ast)
{
    struct ast_element *ast_element = (struct ast_element *)ast;
    print_word(ast_element->word);
}

void ast_pipeline_pretty_print(struct ast *ast)
{
    struct ast_pipeline *ast_pipeline = (struct ast_pipeline *)ast;

    if (!ast_pipeline->istrue)
        printf("! ");

    struct ast *left = ast_pipeline->left;
    ast_pretty_print(left);

    if (ast_pipeline->right)
    {
        printf(" | ");
        struct ast *right = ast_pipeline->right;
        ast_pretty_print(right);
        printf("; ");
    }
}
