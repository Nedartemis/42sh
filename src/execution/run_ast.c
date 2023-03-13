#include "run_ast.h"

#include <assert.h>
#include <err.h>
#include <errno.h>
#include <stdio.h>

#include "ast_pretty_printer.h"
#include "ast_struct.h"
#include "builtin.h"
#include "dico_func.h"
#include "dico_word.h"
#include "expansion.h"
#include "functions.h"
#include "list.h"
#include "ope_string.h"
#include "pipe.h"
#include "redirection.h"
#include "run_ast2.h"
#include "sh_instance.h"
#include "simple_commands.h"
#include "struct.h"
#include "tools.h"

static bool continue_break_condition(void)
{
    struct sh *sh = sh_instance_get();
    return sh->builtin_type == CONTINUE || sh->builtin_type == BREAK;
}

static bool ast_is_loop(enum AstType type)
{
    return type == AST_WHILE || type == AST_FOR;
}

int run_ast(struct ast *ast)
{
    struct sh *sh = sh_instance_get();

    if (continue_break_condition())
    {
        if (!ast_is_loop(ast->type))
            return sh->return_code;
    }

    if (sh->need_to_exit)
        return sh->return_code;

    int return_code = ast->vtable->run(ast);

    if (sh->need_to_exit)
        return sh->return_code;

    sh->return_code = return_code;
    return return_code;
}

int run_redir(struct ast *ast)
{
    assert(ast->type == AST_REDIR);

    struct ast_redir *ast_redir = (struct ast_redir *)ast;

    return redirect(ast_redir);
}

int run_if(struct ast *ast)
{
    assert(ast->type == AST_IF);

    struct ast_if *ast_if = (struct ast_if *)ast;
    int condition = run_ast(ast_if->conditions);

    int return_code = TRUE;
    if (condition == TRUE)
    {
        return_code = run_ast(ast_if->if_body);
    }
    else if (ast_if->else_body)
    {
        return_code = run_ast(ast_if->else_body);
    }

    return return_code;
}

// TODO check SCL for how to manage RETURN CODE
int run_list(struct ast *ast)
{
    assert(ast->type == AST_LIST);
    struct ast_list *ast_l = (struct ast_list *)ast;
    size_t size = list_size(ast_l->asts);
    int return_code = 1;

    for (size_t i = 0; i < size; i++)
    {
        // Get the i'th child in ast's asts ast list
        struct ast *child_ast = (struct ast *)list_get(ast_l->asts, i);
        return_code = run_ast(child_ast);
    }

    return return_code;
}

static int run_prefixes(struct list *ast_prefixes)
{
    size_t size = list_size(ast_prefixes);
    int return_code = 0;
    for (size_t i = 0; i < size; i++)
    {
        struct ast *ast = (struct ast *)list_get(ast_prefixes, i);
        return_code = run_prefix(ast);
    }
    return return_code;
}

static void update_args_env(char **words, struct sh *sh)
{
    size_t size = 0;

    for (size_t i = 1; words[i]; i++)
        size++;

    sh->special_env->argc = size;
    sh->special_env->argv = words + 1;
}

int run_cmd(struct ast *ast)
{
    struct sh *sh = sh_instance_get();

    assert(ast->type == AST_CMD);
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;

    int return_code = 0;
    if (list_empty(ast_cmd->words))
        goto end;

    char **words = list_expend_to_arr(ast_cmd->words);

    // Case simple var multiple words
    treat_env_var(ast_cmd->ast_prefixes, 0);

    if (!words[0]) // nothing after expand, so nothing to exec
    {
        return_code = sh->return_code;
        goto destroy_var;
    }

    char *command = words[0];

    // execution
    if (dico_func_in(sh->df, command))
    { // case func
        int tmp_argc = sh->special_env->argc;
        char **tmp_argv = sh->special_env->argv;

        // update, run and re-update special env values
        update_args_env(words, sh);
        run_function(sh->df, command);
        sh->special_env->argc = tmp_argc;
        sh->special_env->argv = tmp_argv;
    }
    else if (is_builtin(command))
    { // case builtin
        return_code = run_builtin(words);
        fflush(stdout);
    }
    else
    {
        return_code = run_simple_command(words);
    }

destroy_var:

    treat_env_var(ast_cmd->ast_prefixes, 1);

    for (int i = 0; words[i]; i++)
    {
        free(words[i]);
    }
    free(words);

    return return_code;

end:
    // run prefixes (all the assignments)
    if (!list_empty(ast_cmd->ast_prefixes))
        return_code = run_prefixes(ast_cmd->ast_prefixes);

    return return_code;
}

int run_pipeline(struct ast *ast)
{
    assert(ast->type == AST_PIPELINE);
    struct ast_pipeline *ast_pipeline = (struct ast_pipeline *)ast;

    struct ast *left_ast = ast_pipeline->left;
    struct ast *right_ast = ast_pipeline->right;

    if (!left_ast)
        errx(3, "run pipeline : no left child");

    int return_code;
    if (!right_ast) // CASE only one child, so no pipe
        return_code = run_ast(left_ast);
    else // case PIPE
        return_code = exec_pipe(left_ast, right_ast);
    if (ast_pipeline->istrue)
        return return_code;
    return return_code == TRUE ? FALSE : TRUE;
}

int run_and_or(struct ast *ast)
{
    assert(ast->type == AST_AND_OR);
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast;

    struct ast *left_ast = ast_and_or->left;
    struct ast *right_ast = ast_and_or->right;
    enum TokenType type = ast_and_or->type;

    if (!left_ast)
        errx(3, "run and_or : no left child");

    // CASE only one child
    if (!right_ast)
        return run_ast(left_ast);

    bool b;

    // case &&
    if (type == AND_IF)
        b = ((run_ast(left_ast) == TRUE) && (run_ast(right_ast) == TRUE));
    else // case ||
        b = ((run_ast(left_ast) == TRUE) || (run_ast(right_ast) == TRUE));
    return b ? TRUE : FALSE;
}

int run_while(struct ast *ast)
{
    assert(ast->type == AST_WHILE);
    struct ast_while *ast_while = (struct ast_while *)ast;

    enum TokenType type = ast_while->type;
    struct ast *conditions = ast_while->conditions;
    struct ast *body = ast_while->body;

    int b = TRUE;
    if (type == UNTIL)
        b = FALSE;

    int return_code = 0;

    struct sh *sh = sh_instance_get();
    sh->nbloop++;
    int cond = run_ast(conditions);
    while (cond == b)
    {
        return_code = run_ast(body);
        // BREAK
        if (sh->builtin_type == BREAK)
        {
            sh->nbtomanage -= 1;
            if (sh->nbtomanage == 0)
                sh->builtin_type = NOTHING;
            break;
        }
        // CONTINUE
        enum BuiltinType type = sh->builtin_type;
        if (type == CONTINUE)
        {
            if (sh->nbtomanage > 1)
                break;
            sh->builtin_type = NOTHING;
        }

        cond = run_ast(conditions);
        if (cond != b) // end loop
            sh->builtin_type = type;
    }
    sh->nbloop--;

    if (sh->builtin_type == CONTINUE)
    {
        sh->nbtomanage -= 1;
        if (sh->nbtomanage == 0)
            sh->builtin_type = NOTHING;
    }

    return return_code;
}

int run_for(struct ast *ast)
{
    assert(ast->type == AST_FOR);
    struct ast_for *ast_for = (struct ast_for *)ast;
    struct sh *sh = sh_instance_get();

    char *var = ast_for->var;

    int return_code = 0;

    // if no words -> words = $@
    struct list *expended_words;
    if (!ast_for->words)
        expended_words = expansion("$@", false);
    else
        expended_words = list_expend_to_list(ast_for->words);

    // get len of ast_for->words
    int len = list_size(expended_words);

    sh->nbloop++;
    for (int i = 0; i < len; i++)
    {
        // gives the value list[i] to var
        dico_word_insert(sh->dv, var, list_get(expended_words, i));
        return_code = run_ast(ast_for->body);

        // BREAK
        if (sh->builtin_type == BREAK)
        {
            sh->nbtomanage -= 1;
            if (sh->nbtomanage == 0)
                sh->builtin_type = NOTHING;
            break;
        }
        // CONTINUE
        if (sh->builtin_type == CONTINUE && i + 1 < len)
        {
            if (sh->nbtomanage > 1)
                break;
            sh->builtin_type = NOTHING;
        }
    }
    sh->nbloop--;

    if (sh->builtin_type == CONTINUE)
    {
        sh->nbtomanage -= 1;
        if (sh->nbtomanage == 0)
            sh->builtin_type = NOTHING;
    }

    list_destroy(expended_words, destroy_string);

    return return_code;
}

int run_prefix(struct ast *ast)
{
    assert(ast->type == AST_PREFIX);
    struct ast_prefix *ast_prefix = (struct ast_prefix *)ast;

    assert(ast_prefix->ass_word != NULL);

    // run function assign_word
    return assign_word(ast_prefix->ass_word);
}

/*
int run_func(struct ast *ast)
{
    assert(ast->type == AST_FUNC);
    struct ast_func *ast_func = (struct ast_func *)ast;

    char *name = ast_func->name;
    struct ast *body = ast_func->body;

    // TODO

    printf("ast_func->name :%s\n", name);
    printf("ast_func->body :");
    ast_pretty_print(body);
    printf("\n");

    return 0;
}
*/
