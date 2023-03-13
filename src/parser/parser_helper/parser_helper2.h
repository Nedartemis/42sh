#ifndef PARSER_HELPER2_H
#define PARSER_HELPER2_H

#include "ast_struct.h"
#include "lexer.h"
#include "list.h"
#include "parser.h"

// parser_helper2.h
bool check_ass(struct parser *parser);
char *eat_ass(struct parser *parser);
struct ast *parse_redirect(struct sh *sh, struct ast *ast_cmd);
bool check_ll2(struct parser *parser, enum TokenType type,
               bool must_or_could_be_reserved);

#endif /* !PARSER_HELPER2_H */
