#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "list.h"
#include "struct.h"

// parser.c
struct parser *parser_init(struct sh *sh);
void parser_run(struct sh *sh);
void parser_destroy(struct parser *parser);

#endif /* !PARSER_H */
