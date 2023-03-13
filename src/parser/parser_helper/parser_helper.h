#ifndef PARSER_HELPER_H
#define PARSER_HELPER_H

#include "ast_struct.h"
#include "lexer.h"
#include "list.h"
#include "parser.h"

enum RuleType
{
    RINPUT,
    RLIST,
    RAND_OR,
    RPIPELINE,
    RCOMMAND,
    RSIMPLE_COMMAND,
    RSHELL_COMMAND,
    RRULE_IF,
    RELSE_CLAUSE,
    RCOMPOUND_LIST,
    RELEMENT,
    RPREFIX,
    RREDIR,
    RWHILE,
    RFOR,
    RFUNCDEC,
    RCASE,
    // RCLAUSE,
    RITEM,
};

// parser_helper.h
void init_print_parser_helper(bool p);
struct ast *parse(struct sh *sh, enum RuleType type);
void print_not_generated(char *str);
void print_tabs(char *str, int nbrec);
bool check(struct parser *parser, enum TokenType type, bool reserved);
bool match(struct parser *parser, enum TokenType type, bool reserved);
bool check_redir(struct parser *parser);
int eat_io_number(struct parser *parser);
enum TokenType eat_redir(struct parser *parser);
char *eat_word(struct parser *parser);

#endif /* !PARSER_HELPER_H */
