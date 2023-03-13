#include "parser.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "ast_graphviz.h"
#include "ast_struct.h"
#include "log.h"
#include "parse_rule.h"
#include "parser_helper.h"

struct parser *parser_init(struct sh *sh)
{
    // allocate memory parser
    struct parser *parser = calloc(1, sizeof(struct parser));
    if (!parser)
        errx(3, "parser_init: not enough memory");

    // init lexer
    parser->lexer = lexer_init(sh->log);

    // init attribute of parser
    parser->nbrec = 0;
    log_mess(sh, "print log%d\n", sh->log);
    parser->log = sh->log;
    init_print_parser_helper(sh->log);

    return parser;
}

void parser_run(struct sh *sh)
{
    log_mess(sh, "----------- Lexer and Parser -------------\n");

    // calls parsing of input
    sh->root_ast = parse_input(sh);
    struct ast *ast = sh->root_ast;

    log_mess(sh, "----------- Ast -------------\n");

    if (!ast)
        log_mess(sh, "NULL ast\n");
    else if (sh->log)
    {
        ast->vtable->pretty_print(ast);
        printf("\n");
        if (sh->log && false)
            ast_create_graph_file(ast);
    }
}

void parser_destroy(struct parser *parser)
{
    // free attribute
    lexer_destroy(parser->lexer);

    // free itself
    free(parser);
}
