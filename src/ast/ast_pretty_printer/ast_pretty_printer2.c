#include "ast_pretty_printer2.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "ast_destroy.h"
#include "ast_destroy2.h"
#include "ast_pretty_printer.h"
#include "ast_struct.h"
#include "err.h"
#include "list.h"
#include "token.h"

void ast_redir_pretty_print(struct ast *ast)
{
    struct ast_redir *ast_redir = (struct ast_redir *)ast;

    printf("(");
    ast_pretty_print(ast_redir->cmd);

    printf(" %d", ast_redir->io_number);
    token_print_type(ast_redir->redir_type);
    printf(" %s; )", ast_redir->word);
    // printf(")");
}

void ast_and_or_pretty_print(struct ast *ast)
{
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast;

    // printf("(");
    ast_pretty_print(ast_and_or->left);
    if (!ast_and_or->right)
        return;
    printf(" ");
    token_print_type(ast_and_or->type);
    printf(" ");
    ast_pretty_print(ast_and_or->right);
    // printf(")");
}

void ast_while_pretty_print(struct ast *ast)
{
    struct ast_while *ast_while = (struct ast_while *)ast;

    token_print_type(ast_while->type);
    printf(" ");
    ast_pretty_print(ast_while->conditions);
    printf(" do\n");
    pretty_print_tabs();
    ast_pretty_print(ast_while->body);
    printf("\ndone");
}

void ast_for_pretty_print(struct ast *ast)
{
    struct ast_for *ast_for = (struct ast_for *)ast;

    printf("for ");
    print_word(ast_for->var);
    if (ast_for->words)
    {
        printf(" in ");
        list_print(ast_for->words, print_word);
    }
    printf(" do\n");
    pretty_print_tabs();
    ast_pretty_print(ast_for->body);
    printf("\ndone");
}

void ast_prefix_pretty_print(struct ast *ast)
{
    struct ast_prefix *ast_prefix = (struct ast_prefix *)ast;

    printf("<%s>", ast_prefix->ass_word);
}

void ast_func_pretty_print(struct ast *ast)
{
    struct ast_func *ast_func = (struct ast_func *)ast;

    printf("%s ()\n{\n", ast_func->name);
    ast_pretty_print(ast_func->body);
    printf("}\n");
}

void ast_subshell_pretty_print(struct ast *ast)
{
    struct ast_subshell *ast_subshell = (struct ast_subshell *)ast;

    printf("(");
    ast_pretty_print(ast_subshell->ast_list);
    printf(")\n");
}

void ast_case_pretty_print(struct ast *ast)
{
    struct ast_case *ast_case = (struct ast_case *)ast;

    printf("case %s in\n", ast_case->word);

    size_t l = list_size(ast_case->items);
    for (size_t i = 0; i < l; i++)
    {
        struct ast *item = list_get(ast_case->items, i);
        ast_pretty_print(item);
    }

    printf("esac\n");
}

void ast_item_pretty_print(struct ast *ast)
{
    struct ast_item *ast_item = (struct ast_item *)ast;

    size_t l = list_size(ast_item->words);
    if (l == 0)
        errx(4, "ast_item_pretty_print: words list empty");

    char *word = list_get(ast_item->words, 0);
    printf("(%s", word);

    for (size_t i = 1; i < l; i++)
    {
        word = list_get(ast_item->words, i);
        printf(" | %s", word);
    }
    printf(")");

    ast_pretty_print(ast_item->body);

    printf("\n");
}
