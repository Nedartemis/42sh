#include <ctype.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "lexer_quoting.h"
#include "lexer_read_input.h"
#include "ope_string.h"
#include "special_var.h"
#include "tools.h"

void lex_dollar_bracket(struct lexer *lexer)
{
    add_to_lexem(lexer, '{');
    while (!is_at_end(lexer) && peek(lexer) != '}')
    {
        char c = advance(lexer);
        if (c == '"')
        {
            lex_double_quote(lexer);
        }
        else if (c == '\'')
            lex_single_quote(lexer);
        else if (c == '\\')
        {
            lex_backslash(lexer);
        }
        else
        {
            add_to_lexem(lexer, c);
        }
    }

    if (!lexer_match(lexer, '}'))
        errx(2, "Error syntax : Open bracket without the close one");
    add_to_lexem(lexer, '}');
}

void lex_dollar_without_bracket(struct lexer *lexer)
{
    if (is_special_char(peek(lexer)))
    {
        add_to_lexem(lexer, advance(lexer));
        return;
    }

    if (!valid_begin_var(peek(lexer)))
        return;

    while (!is_at_end(lexer))
    {
        char c = peek(lexer);
        if (is_blank(c) || !is_char_name_var(c))
            break;
        add_to_lexem(lexer, advance(lexer));
    }
}
