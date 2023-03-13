#ifndef PARSE_RULE_H
#define PARSE_RULE_H

#include "ast.h"
#include "parser.h"

struct ast *parse_input(struct sh *sh);
struct ast *parse_list(struct sh *sh);
struct ast *parse_and_or(struct sh *sh);
// struct ast *parse_and_or(struct sh *sh);
struct ast *parse_pipeline(struct sh *sh);
struct ast *parse_command(struct sh *sh);
struct ast *parse_simple_command(struct sh *sh);
struct ast *parse_shell_command(struct sh *sh);
struct ast *parse_rule_if(struct sh *sh);
struct ast *parse_else_clause(struct sh *sh);

#endif /* !PARSE_RULE_H */
