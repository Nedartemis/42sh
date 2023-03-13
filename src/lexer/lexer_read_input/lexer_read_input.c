#include "lexer_read_input.h"

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer.h"
#include "list.h"
#include "log.h"
#include "token.h"

bool is_end(char c)
{
    return c == EOF;
}

bool is_at_end(struct lexer *lexer)
{
    return is_end(peek(lexer));
}

void add_to_lexem(struct lexer *lexer, char c)
{
    assert(c != '\0');
    vector_append(lexer->lexem, c);
}

bool is_blank(char c)
{
    return c == ' ' || c == '\t' || c == '\r';
}

// -------------- Read input ---------------------

char peek(struct lexer *lexer)
{
    if (lexer->need_to_read)
    {
        if (lexer->annexe_input)
        {
            lexer->current = fgetc(lexer->annexe_input);
            assert(lexer->current != 0);
            if (lexer->current == EOF)
            {
                free(lexer->annexe_str);
                lexer->annexe_str = NULL;
                fclose(lexer->annexe_input);
                lexer->annexe_input = NULL;
                lexer->current = ' ';
            }
        }
        else
            lexer->current = fgetc(lexer->input);

        log_messp(lexer->log, "--- c = '%c'\n", lexer->current);
        lexer->need_to_read = false;
        return lexer->current;
    }

    if (is_end(lexer->current))
        return EOF;
    return lexer->current;
}

bool lexer_check(struct lexer *lexer, char c)
{
    return peek(lexer) == c;
}

char advance(struct lexer *lexer)
{
    char current = peek(lexer);
    lexer->need_to_read = true;
    return current;
}

bool lexer_match(struct lexer *lexer, char expected)
{
    if (is_at_end(lexer))
        return false;

    if (peek(lexer) != expected)
        return false;

    advance(lexer);
    return true;
}
