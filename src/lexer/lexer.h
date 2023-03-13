#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

enum TokenType
{
    // reserved words
    IF,
    THEN,
    ELIF,
    ELSE,
    FI,
    BANG,
    WHILE,
    UNTIL,
    FOR,
    DO,
    DONE,
    IN,
    LBRACKET,
    RBRACKET,
    CASE,
    ESAC,

    NEWLINE,
    LPAR,
    RPAR,
    ENDOF,
    IO_NUMBER,
    ASSIGNMENT,

    // operator
    SEMICOLON,
    DSEMICOLON, // ;;
    GREAT,
    LESS,
    DGREAT,
    GREATAND,
    LESSAND,
    CLOBBER,
    LESSGREAT,
    PIPE,
    AND_IF,
    OR_IF,

    // word
    LWORD,

};

struct token
{
    enum TokenType type;
    char *lexem;
};

struct lexer
{
    struct list *tokens;
    FILE *input;
    struct vector *lexem;
    char current; // set to '\0'
    bool log;
    bool need_to_read; // set to true
    bool part_of_word; // set to false
    bool quoted; // set to false
    int error; // set to 0
    char *annexe_str;
    FILE *annexe_input;
};

// lexer.c

struct lexer *lexer_init(bool log);
void lexer_give_input(struct lexer *lexer, FILE *input);
void lexer_destroy(struct lexer *lexer);
void lexer_print(struct lexer *lexer);
void lexer_reset(struct lexer *lexer);

// lexer_test.c

int lexer_test(int argc, char *argv[]);

#endif /* !LEXER_H */
