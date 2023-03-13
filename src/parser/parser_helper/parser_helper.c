#include "parser_helper.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "ast_struct.h"
#include "lexer.h"
#include "lexer_run.h"
#include "log.h"
#include "parse_rule.h"
#include "parse_rule2.h"
#include "parser_helper2.h"
#include "token.h"

static bool print = false;

typedef struct ast *(*func_parse)(struct sh *);

static const func_parse rule_func[] = {
    [RINPUT] = parse_input,
    [RLIST] = parse_list,
    [RAND_OR] = parse_and_or,
    [RPIPELINE] = parse_pipeline,
    [RCOMMAND] = parse_command,
    [RSIMPLE_COMMAND] = parse_simple_command,
    [RSHELL_COMMAND] = parse_shell_command,
    [RRULE_IF] = parse_rule_if,
    [RELSE_CLAUSE] = parse_else_clause,
    [RCOMPOUND_LIST] = parse_compound_list,
    [RELEMENT] = parse_element,
    [RPREFIX] = parse_prefix,
    [RREDIR] = parse_redir,
    [RWHILE] = parse_while,
    [RFOR] = parse_for,
    [RFUNCDEC] = parse_funcdec,
    [RCASE] = parse_case,
    //[RCLAUSE] = parse_clause,
    [RITEM] = parse_item,
};

static char *rule_str[] = {
    [RINPUT] = "input",
    [RLIST] = "list",
    [RAND_OR] = "and_or",
    [RPIPELINE] = "pipeline",
    [RCOMMAND] = "command",
    [RSIMPLE_COMMAND] = "simple_command",
    [RSHELL_COMMAND] = "shell_command",
    [RRULE_IF] = "rule_if",
    [RELSE_CLAUSE] = "else_clause",
    [RCOMPOUND_LIST] = "compound_list",
    [RELEMENT] = "element",
    [RPREFIX] = "prefix",
    [RREDIR] = "redir",
    [RWHILE] = "while",
    [RFOR] = "for",
    [RFUNCDEC] = "funcdec",
    [RCASE] = "case",
    //[RCLAUSE] = "clause",
    [RITEM] = "item",
};

void init_print_parser_helper(bool p)
{
    print = p;
}

struct ast *parse(struct sh *sh, enum RuleType type)
{
    if (sh->return_code == 2)
        return NULL;

    struct parser *parser = sh->parser;

    print_tabs(rule_str[type], parser->nbrec++);
    struct ast *res = rule_func[type](sh);
    print_tabs(rule_str[type], --parser->nbrec);

    if (sh->return_code == 2)
    {
        if (res)
            res->vtable->destroy(res);
        return NULL;
    }

    return res;
}

void print_not_generated(char *str)
{
    log_messp(print, "================= %s not generated ================\n",
              str);
}

void print_tabs(char *str, int nbrec)
{
    if (!str)
        return;
    for (int i = 0; i < nbrec; i++)
        log_messp(print, "\t");
    log_messp(print, "%s\n", str);
}

bool check(struct parser *parser, enum TokenType type,
           bool must_or_could_be_reserved)
{
    // get token
    if (!lexer_check_token(parser->lexer, type, must_or_could_be_reserved))
        return false;

    return true;
}

bool match(struct parser *parser, enum TokenType type,
           bool must_or_could_be_reserved)
{
    if (!check(parser, type, must_or_could_be_reserved))
        return false;

    // get token
    if (print)
    {
        printf("Token match = ");
        token_print_type(type);
        printf("\n");
    }

    // move forward
    lexer_move_forward(parser->lexer);
    return true;
}

static bool is_redir_type(enum TokenType token_type)
{
    return (token_type == DGREAT || token_type == LESSAND
            || token_type == GREATAND || token_type == LESSGREAT
            || token_type == CLOBBER || token_type == LESS
            || token_type == GREAT);
}

// check if a token is a redir
bool check_redir(struct parser *parser)
{
    // get first token
    enum TokenType token_type =
        lexer_get_token_type(parser->lexer, false, false);

    bool is_redir = is_redir_type(token_type);

    return is_redir;
}

// free token and return lexem (when token = WORD)
char *eat_word(struct parser *parser)
{
    // get first token
    char *lexem = lexer_get_token_lexem(parser->lexer);

    // move forward
    lexer_move_forward(parser->lexer);

    return lexem;
}

// free token and return io_number
int eat_io_number(struct parser *parser)
{
    // check first token
    assert(lexer_get_token_type(parser->lexer, false, false) == IO_NUMBER);

    // transforms lexem into int
    char *lexem = lexer_get_token_lexem(parser->lexer);
    int io_number = atoi(lexem);

    free(lexem);

    // move forward
    lexer_move_forward(parser->lexer);

    return io_number;
}

// free token and return redir_type
enum TokenType eat_redir(struct parser *parser)
{
    // get redir_type
    enum TokenType redir_type =
        lexer_get_token_type(parser->lexer, false, false);

    // check if redir_type
    assert(is_redir_type(redir_type));

    // move forward
    lexer_move_forward(parser->lexer);

    return redir_type;
}
