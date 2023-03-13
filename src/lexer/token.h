#ifndef TOKEN_H
#define TOKEN_H

#include "lexer.h"

// token.c
struct token *token_init(enum TokenType type, char *lexem);
void token_print_data(void *data);
void token_print_type(enum TokenType type);
void token_print_type_pretty(enum TokenType type);
void token_print(struct token *token);
enum TokenType word_to_reserved_word(char *lexem);
bool is_reserved_type(enum TokenType type);
void token_expension_single_quote(struct token *token);
void token_destroy(struct token *token);
void token_destroy_data(void *data);

#endif /* !TOKEN_H */
