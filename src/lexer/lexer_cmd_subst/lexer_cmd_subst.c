#include <assert.h>
#include <err.h>
#include <stdio.h>

#include "lexer.h"
#include "lexer_quoting.h"
#include "lexer_read_input.h"
#include "lexer_run.h"

static void lex_par_func(struct lexer *lexer)
{
    add_to_lexem(lexer, '(');
    while (!is_at_end(lexer) && peek(lexer) != ')')
    {
        add_to_lexem(lexer, advance(lexer));
    }

    if (is_at_end(lexer))
        errx(2,
             "Lexical error : open parenthesis for func without the close one");
    add_to_lexem(lexer, advance(lexer));
}

void lex_dollar_par(struct lexer *lexer)
{
    add_to_lexem(lexer, '(');
    while (!is_at_end(lexer) && peek(lexer) != ')')
    {
        char c = advance(lexer);
        if (c == '"')
        {
            lex_double_quote(lexer);
        }
        else if (c == '\'')
        {
            lex_single_quote(lexer);
        }
        else if (c == '\\')
        {
            lex_backslash(lexer);
        }
        else if (c == '$')
        {
            lex_dollar(lexer);
        }
        else if (c == '(')
        {
            lex_par_func(lexer);
        }
        else
        {
            add_to_lexem(lexer, c);
        }
    }

    if (!lexer_match(lexer, ')'))
        errx(2, "Error syntax : Open par for cmd subst without the close one");
    add_to_lexem(lexer, ')');
}

static void lex_espape_backquote(struct lexer *lexer)
{
    assert(advance(lexer) == '`');
    while (!is_at_end(lexer))
    {
        char c = advance(lexer);
        if (c == '\\')
        {
            if (lexer_match(lexer, '`'))
                return;
            advance(lexer);
        }
    }

    errx(2, "Error syntax : Open escaped backquote without the close one");
}

void lex_backquote(struct lexer *lexer)
{
    add_to_lexem(lexer, '`');
    while (!is_at_end(lexer) && peek(lexer) != '`')
    {
        char c = advance(lexer);
        if (c == '"')
        {
            lex_double_quote(lexer);
        }
        else if (c == '\'')
        {
            lex_single_quote(lexer);
        }
        else if (c == '\\')
        {
            if (peek(lexer) == '$')
                continue;
            if (peek(lexer) == '\\')
            {
                add_to_lexem(lexer, advance(lexer));
                continue;
            }
            else if (peek(lexer) == '`')
            {
                lex_espape_backquote(lexer);
                continue;
            }

            lex_backslash(lexer);
        }
        else if (c == '$')
        {
            lex_dollar(lexer);
        }
        else
        {
            add_to_lexem(lexer, c);
        }
    }

    if (!lexer_match(lexer, '`'))
        errx(2, "Error syntax : Open backquote without the close one");
    add_to_lexem(lexer, '`');
}
