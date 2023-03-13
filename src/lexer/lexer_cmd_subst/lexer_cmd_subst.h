#ifndef LEXER_CMD_SUBST_H
#define LEXER_CMD_SUBST_H

#include "lexer.h"

void lex_dollar_par(struct lexer *lexer);
void lex_backquote(struct lexer *lexer);

#endif /* ! LEXER_CMD_SUBST_H */
