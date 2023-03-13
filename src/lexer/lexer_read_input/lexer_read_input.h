#ifndef LEXER_HELPER_H
#define LEXER_HELPER_H

#include <stdbool.h>

#include "lexer.h"

bool is_at_end(struct lexer *lexer);
void add_to_lexem(struct lexer *lexer, char c);
bool is_blank(char c);

// read input
char peek(struct lexer *lexer);
bool lexer_check(struct lexer *lexer, char c);
char advance(struct lexer *lexer);
bool lexer_match(struct lexer *lexer, char expected);

#endif /* !LEXER_HELPER_H */
