#ifndef PARSE_RULE2_H
#define PARSE_RULE2_H

#include "ast.h"
#include "parser.h"

struct ast *parse_compound_list(struct sh *sh);
struct ast *parse_element(struct sh *sh);
struct ast *parse_prefix(struct sh *sh);
struct ast *parse_redir(struct sh *sh);
struct ast *parse_while(struct sh *sh);
struct ast *parse_for(struct sh *sh);
struct ast *parse_funcdec(struct sh *sh);
struct ast *parse_case(struct sh *sh);
struct list *parse_clause(struct sh *sh);
struct ast *parse_item(struct sh *sh);

#endif /* !PARSE_RULE2_H */
