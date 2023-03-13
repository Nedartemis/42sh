#include <err.h>
#include <stdbool.h>
#include <stdio.h>

#include "lexer.h"
#include "lexer_read_input.h"
#include "lexer_run.h"
#include "null_array.h"

bool beginning_of_ope(char c)
{
    const int n = 5;
    const char array[] = { ';', '>', '<', '|', '&' };

    for (int i = 0; i < n; i++)
    {
        if (array[i] == c)
            return true;
    }
    return false;
}

bool beginning_of_redir(char c)
{
    const char arr[] = { '>', '<', 0 };
    return array_char_in(arr, c);
}

static char ope_lex_backslash(struct lexer *lexer, char c)
{
    if (c != '\\')
        return c;

    advance(lexer);
    if (advance(lexer) != '\n')
        errx(7,
             "Error manage_operator : Operator not handle (begin with a '\\')");
    return peek(lexer);
}

static enum TokenType manage_great(struct lexer *lexer, bool *need_to_advance,
                                   char peek)
{
    peek = ope_lex_backslash(lexer, peek);

    if (peek == '>')
    {
        *need_to_advance = true;
        return DGREAT;
    }
    else if (peek == '&')
    {
        *need_to_advance = true;
        return GREATAND;
    }
    else if (peek == '|')
    {
        *need_to_advance = true;
        return CLOBBER;
    }
    else
    {
        *need_to_advance = false;
        return GREAT;
    }
}

static enum TokenType manage_less(struct lexer *lexer, bool *need_to_advance,
                                  char peek)
{
    peek = ope_lex_backslash(lexer, peek);
    if (peek == '&')
    {
        *need_to_advance = true;
        return LESSAND;
    }
    else if (peek == '>')
    {
        *need_to_advance = true;
        return LESSGREAT;
    }
    else
    {
        *need_to_advance = false;
        return LESS;
    }
}

static enum TokenType manage_and_if(struct lexer *lexer, bool *need_to_advance,
                                    char peek)
{
    peek = ope_lex_backslash(lexer, peek);
    if (peek != '&')
        errx(2, "Error : '&%c' not implemented", peek);
    *need_to_advance = true;
    return AND_IF;
}

static enum TokenType manage_pipe(struct lexer *lexer, bool *need_to_advance,
                                  char peek)
{
    peek = ope_lex_backslash(lexer, peek);
    if (peek == '|')
    {
        *need_to_advance = true;
        return OR_IF;
    }
    else
    {
        *need_to_advance = false;
        return PIPE;
    }
}

// true if it genrat an operator
void manage_operator(struct lexer *lexer, char current)
{
    enum TokenType type;
    char c = peek(lexer);
    bool need_to_advance = false;

    switch (current)
    {
    case ';': // don't move
        if (c == ';')
        {
            type = DSEMICOLON;
            need_to_advance = true;
        }
        else
            type = SEMICOLON;
        break;
    case '>':
        type = manage_great(lexer, &need_to_advance, c);
        break;
    case '<':
        type = manage_less(lexer, &need_to_advance, c);
        break;
    case '&':
        type = manage_and_if(lexer, &need_to_advance, c);
        break;
    case '|':
        type = manage_pipe(lexer, &need_to_advance, c);
        break;
    default:
        errx(8, "Error manage_operator : Operator not handle");
        break;
    }

    if (need_to_advance)
        advance(lexer);

    // delimit
    add_token(lexer, type);
}
