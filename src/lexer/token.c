#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

static const char *token_to_string[] = {
    // reserved word
    [IF] = "IF",
    [THEN] = "THEN",
    [ELIF] = "ELIF",
    [ELSE] = "ELSE",
    [FI] = "FI",
    [BANG] = "BANG",
    [WHILE] = "WHILE",
    [UNTIL] = "UNTIL",
    [FOR] = "FOR",
    [DO] = "DOOOOOOO",
    [DONE] = "DONE",
    [IN] = "IN",
    [LBRACKET] = "LBRACKET",
    [RBRACKET] = "RBRACKET",
    [CASE] = "CASE",
    [ESAC] = "ESAC",
    //

    [NEWLINE] = "NEWLINE",
    [ENDOF] = "EOF",
    [LWORD] = "LWORD",
    [IO_NUMBER] = "IO_NUMBER",
    [ASSIGNMENT] = "ASSIGNMENT",
    [LPAR] = "LPAR",
    [RPAR] = "RPAR",

    // operator
    [SEMICOLON] = "SEMICOLON",
    [DSEMICOLON] = "DSEMICOLON",
    [GREAT] = ">",
    [LESS] = "<",
    [DGREAT] = ">>",
    [GREATAND] = ">&",
    [LESSAND] = "<&",
    [CLOBBER] = ">|",
    [LESSGREAT] = "<>",
    [PIPE] = "|",
    [AND_IF] = "&&",
    [OR_IF] = "||",
};

struct token *token_init(enum TokenType type, char *lexem)
{
    // create token
    struct token *token = calloc(1, sizeof(struct token));
    token->type = type;
    token->lexem = lexem;

    if (type == LWORD && (!lexem || lexem[0] == '\0'))
        errx(3, "LWORD should never have an empty lexem");

    return token;
}

void token_print_type(enum TokenType type)
{
    printf("%s", token_to_string[type]);
}

void token_print_type_pretty(enum TokenType type)
{
    printf("type = ");
    token_print_type(type);
    printf("\n");
}

void token_print(struct token *token)
{
    if (!token)
    {
        printf("(null)\n");
        return;
    }

    if (token->type == LWORD)
    {
        printf("'%s'", token->lexem);
        return;
    }

    token_print_type(token->type);
}

void token_print_data(void *data)
{
    token_print((struct token *)data);
}

enum TokenType word_to_reserved_word(char *lexem)
{
    if (strcmp(lexem, "if") == 0)
        return IF;
    if (strcmp(lexem, "elif") == 0)
        return ELIF;
    if (strcmp(lexem, "then") == 0)
        return THEN;
    if (strcmp(lexem, "else") == 0)
        return ELSE;
    if (strcmp(lexem, "fi") == 0)
        return FI;
    if (strcmp(lexem, "!") == 0)
        return BANG;
    if (strcmp(lexem, "while") == 0)
        return WHILE;
    if (strcmp(lexem, "until") == 0)
        return UNTIL;
    if (strcmp(lexem, "for") == 0)
        return FOR;
    if (strcmp(lexem, "do") == 0)
        return DO;
    if (strcmp(lexem, "done") == 0)
        return DONE;
    if (strcmp(lexem, "in") == 0)
        return IN;
    if (strcmp(lexem, "{") == 0)
        return LBRACKET;
    if (strcmp(lexem, "}") == 0)
        return RBRACKET;
    if (strcmp(lexem, "case") == 0)
        return CASE;
    if (strcmp(lexem, "esac") == 0)
        return ESAC;

    return LWORD;
}

bool is_reserved_type(enum TokenType type)
{
    static const enum TokenType reserved_word[] = {
        IF, THEN, ELIF, ELSE,     FI,       BANG, WHILE, UNTIL, FOR,
        DO, DONE, IN,   LBRACKET, RBRACKET, CASE, ESAC,  ENDOF
    };

    for (int i = 0; reserved_word[i] != ENDOF; i++)
    {
        if (reserved_word[i] == type)
            return true;
    }
    return false;
}

void token_expension_single_quote(struct token *token)
{
    if (!token->lexem)
        return;

    char buf[2048];
    int ibuf = 0;
    for (int i = 0; token->lexem[i] != 0; i++)
    {
        if (token->lexem[i] == '\'')
            continue;

        buf[ibuf++] = token->lexem[i];
    }

    buf[ibuf] = 0;
    char *to_destroy = token->lexem;
    token->lexem = strndup(buf, ibuf);
    free(to_destroy);
}

void token_destroy(struct token *token)
{
    if (!token)
        return;
    free(token->lexem);
    free(token);
}

void token_destroy_data(void *data)
{
    struct token *token = data;
    token_destroy(token);
}
