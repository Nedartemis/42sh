#include "run_ast2.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#include "ast_pretty_printer.h"
#include "ast_struct.h"
#include "builtin.h"
#include "dico_func.h"
#include "expansion.h"
#include "list.h"
#include "ope_string.h"
#include "pipe.h"
#include "redirection.h"
#include "run_ast.h"
#include "sh_instance.h"
#include "simple_commands.h"
#include "struct.h"
#include "subshell.h"
#include "tools.h"

int run_func(struct ast *ast)
{
    assert(ast->type == AST_FUNC);
    struct ast_func *ast_func = (struct ast_func *)ast;

    if (ast_func->body)
    {
        char *name = ast_func->name;
        struct sh *sh = sh_instance_get();

        dico_func_insert(sh->df, name, ast_func->body);
        ast_func->body = NULL;
    }

    return 0;
}

int run_subshell(struct ast *ast)
{
    assert(ast->type == AST_SUBSHELL);
    struct ast_subshell *ast_subshell = (struct ast_subshell *)ast;

    struct ast *ast_list = ast_subshell->ast_list;

    int return_code = execute_subshell(ast_list);
    // printf("return_code: %d\n", return_code);

    return return_code;
}

static bool match_word(struct list *words, char *word)
{
    assert(!list_empty(words));
    size_t l = list_size(words);

    for (size_t i = 0; i < l; i++)
    {
        struct list *word_list = expansion(list_get(words, i), false);
        char *word_expanded = strdup(list_get(word_list, 0));
        list_destroy(word_list, destroy_string);

        // check if words[i] == word
        if (strcmp(word_expanded, word) == 0)
        {
            free(word_expanded);
            return true;
        }
        free(word_expanded);
    }
    return false;
}

int run_case(struct ast *ast)
{
    assert(ast->type == AST_CASE);
    struct ast_case *ast_case = (struct ast_case *)ast;

    struct list *items = ast_case->items;
    // no clause
    if (list_empty(items))
        return 0;

    struct list *word_list = expansion(ast_case->word, false);
    char *word = strdup(list_get(word_list, 0));
    list_destroy(word_list, destroy_string);

    int return_code = 0;

    size_t l = list_size(items);

    for (size_t i = 0; i < l; i++)
    {
        struct ast *item = list_get(items, i);
        struct ast_item *ast_item = (struct ast_item *)item;

        // if word match run body of item
        if (match_word(ast_item->words, word))
        {
            free(word);
            return run_ast(ast_item->body);
        }
    }

    free(word);
    return return_code;
}
