#include "sh.h"

#include <stdio.h>
#include <stdlib.h>

// ast/
#include "ast.h"
#include "ast_graphviz.h"
#include "ast_pretty_printer.h"
#include "get_input.h"
// lexer/
#include "lexer.h"
// tools/
#include "dico_func.h"
#include "dico_word.h"
#include "list.h"
#include "tools.h"
// parser/
#include "parser.h"
// execution/
#include "expansion.h"
#include "log.h"
#include "run_ast.h"
#include "sh_instance.h"

struct sh *sh_init(int argc, char *argv[])
{
    struct sh *sh = calloc(1, sizeof(struct sh));

    // init log
    sh->log = false;
    char *str = getenv("PRINT");
    if (str && str[0] == '1')
        sh->log = true;

    // init parser
    sh->parser = parser_init(sh);

    // init attribute
    sh->eof = false;
    sh->return_code = 0;
    sh->argc = argc;
    sh->argv = argv;
    sh->builtin_type = NOTHING;
    sh->need_to_exit = false;

    // dico
    sh->dv = dico_word_init();
    sh->df = dico_func_init();
    sh->da = dico_word_init();

    // init func
    sh->destroy = sh_destroy;

    // init instance in files.c
    sh_instance_init(sh);

    // init special_env
    sh->special_env = calloc(1, sizeof(struct special_env));

    return sh;
}

void sh_reset(struct sh *sh)
{
    sh->eof = false;
    sh->return_code = 0;
    sh->need_to_exit = false;

    lexer_reset(sh->parser->lexer);
}

void sh_copy_dico(struct sh *sh, struct dico_word *dv, struct dico_func *df)
{
    dico_word_destroy(sh->dv);
    dico_func_destroy(sh->df);

    sh->dv = dv;
    sh->df = df;
}

static void sh_execute(struct sh *sh)
{
    if (sh->argc == 1)
        printf("\n42sh$ ");

    // run the parser
    parser_run(sh);
    fflush(sh->parser->lexer->input);

    struct ast *ast = sh->root_ast;

    if (!ast)
        return;

    log_mess(sh, "----------- Execute Ast -------------\n");
    // run the ast
    sh->return_code = ast->vtable->run(ast);

    // destroy the ast
    ast->vtable->destroy(ast);
    sh->root_ast = NULL;
}

int sh_run(struct sh *sh)
{
    // get the input
    FILE *input = get_input(sh);
    if (input == NULL)
    {
        printf("ewqrewrewr");
        sh_destroy(sh);
        return 0;
    }

    // gives the input to the lexer
    lexer_give_input(sh->parser->lexer, input);

    // executes the shell while no EOF detected and no syntax nor lexic error
    // occurs
    while (!sh->eof && sh->return_code != 2 && !sh->need_to_exit)
    {
        sh_execute(sh);
    }

    return sh->return_code;
}

int sh_main(int argc, char *argv[])
{
    // init the sh
    struct sh *sh = sh_init(argc, argv);

    int return_code = sh_run(sh);

    // destroy the sh
    sh_destroy(sh);

    return return_code;
}

void sh_destroy(struct sh *sh)
{
    struct ast *ast = sh->root_ast;
    if (ast)
        ast->vtable->destroy(ast);

    // free parser
    parser_destroy(sh->parser);

    // free dico var and func
    dico_word_destroy(sh->dv);
    dico_func_destroy(sh->df);
    dico_word_destroy(sh->da);

    // free special env
    free(sh->special_env);

    // free itself
    free(sh);
}
