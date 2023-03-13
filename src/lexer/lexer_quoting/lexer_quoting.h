#ifndef QUOTING_H
#define QUOTING_H

#include "lexer.h"

void lex_backslash(struct lexer *lexer);
void lex_single_quote(struct lexer *lexer);
void lex_double_quote(struct lexer *lexer);

#endif /* !QUOTING_H */
