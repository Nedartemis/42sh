#include "lexer_quoting.h"

#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "lexer_read_input.h"
#include "lexer_run.h"
#include "lexer_var.h"
#include "special_var.h"
#include "tools.h"

void lex_backslash(struct lexer *lexer)
{
    if (is_at_end(lexer))
        return;

    char c = advance(lexer);
    if (c != '\n')
    {
        add_to_lexem(lexer, '\\');
        add_to_lexem(lexer, c);
        return;
    }

    // c == '\n' do nothing
}

void lex_single_quote(struct lexer *lexer)
{
    add_to_lexem(lexer, '\'');

    while (!is_at_end(lexer) && peek(lexer) != '\'')
    {
        add_to_lexem(lexer, advance(lexer));
    }

    if (!lexer_match(lexer, '\''))
    {
        lexer->error = 2;
        errx(2, "Error syntax : Open single quote without the close one");
    }

    add_to_lexem(lexer, '\'');
}

void lex_double_quote(struct lexer *lexer)
{
    add_to_lexem(lexer, '"');

    while (!is_at_end(lexer) && peek(lexer) != '"')
    {
        char c = peek(lexer);
        if (c == '\\')
        {
            advance(lexer);
            lex_backslash(lexer);
            continue;
        }
        else if (c == '$')
        {
            advance(lexer);
            lex_dollar(lexer);
        }
        else
            add_to_lexem(lexer, advance(lexer));
    }

    if (!lexer_match(lexer, '"'))
    {
        lexer->error = 2;
        errx(2, "Error syntax : Open double quote without the close one");
    }

    add_to_lexem(lexer, '"');
}
