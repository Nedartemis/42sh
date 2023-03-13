#ifndef LEXER_RUN_H
#define LEXER_RUN_H

#include "lexer.h"

void add_token(struct lexer *lexer, enum TokenType type);
void lexer_move_forward(struct lexer *lexer);

void lex_dollar(struct lexer *lexer);

// LL1

enum TokenType lexer_get_token_type(struct lexer *lexer,
                                    bool must_or_could_be_reserved,
                                    bool is_ass);
bool lexer_check_token(struct lexer *lexer, enum TokenType type_expected,
                       bool must_or_could_be_reserved);
char *lexer_get_token_lexem(struct lexer *lexer);

// LL2

enum TokenType lexer_get_token_type_ll2(struct lexer *lexer,
                                        bool must_or_could_be_reserved,
                                        bool is_ass);
bool lexer_check_token_ll2(struct lexer *lexer, enum TokenType type_expected,
                           bool must_or_could_be_reserved);

#endif /* !LEXER_RUN_H */
