#include "parser_helper2.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "ast_destroy.h"
#include "ast_struct.h"
#include "lexer.h"
#include "lexer_run.h"
#include "log.h"
#include "parser_helper.h"
#include "token.h"

// checks if a word is an ASSIGNMENT
bool check_ass(struct parser *parser)
{
    // get token
    return lexer_check_token(parser->lexer, ASSIGNMENT, false);
}

// free token and move forward
char *eat_ass(struct parser *parser)
{
    // get ass_type
    enum TokenType ass_type = lexer_get_token_type(parser->lexer, false, true);

    // check if ass_type
    assert(ass_type == ASSIGNMENT);

    // get ass_word
    char *ass_word = lexer_get_token_lexem(parser->lexer);

    // move forward
    lexer_move_forward(parser->lexer);

    return ass_word;
}

// handle the { redirection }
struct ast *parse_redirect(struct sh *sh, struct ast *ast_cmd)
{
    // declare ast_redir
    struct ast *ast;

    // storages ast_redir
    struct list *redirs = list_init();

    // add list element
    for (; true;)
    {
        ast = parse(sh, RREDIR);
        if (!ast)
            break;
        list_prepend(redirs, ast);
    }

    while (!list_empty(redirs))
    {
        ast = list_pop_front(redirs);
        assert(ast != NULL);
        struct ast_redir *ast_redir = (struct ast_redir *)ast;
        ast_redir->cmd = ast_cmd;
        ast_cmd = ast;
    }

    list_destroy(redirs, destroy_ast_void);
    // return ast
    print_tabs("stops correctly", sh->parser->nbrec);
    return ast_cmd;
}

// check ll2
bool check_ll2(struct parser *parser, enum TokenType type,
               bool must_or_could_be_reserved)
{
    // get token
    if (!lexer_check_token_ll2(parser->lexer, type, must_or_could_be_reserved))
        return false;

    return true;
}
