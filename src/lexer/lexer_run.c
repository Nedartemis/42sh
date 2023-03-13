#include "lexer_run.h"

#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lexer_alias.h"
#include "lexer_cmd_subst.h"
#include "lexer_quoting.h"
#include "lexer_read_input.h"
#include "lexer_var.h"
#include "list.h"
#include "log.h"
#include "ope_string.h"
#include "operator.h"
#include "token.h"
#include "vector.h"

// ------------- Add token ---------------------

static bool is_an_assignment(const char *lexem)
{
    size_t i;
    for (i = 0; lexem[i] && lexem[i] != '='; i++)
        continue;

    return lexem[i] == '=' && is_valid_name_var(lexem, i);
}

static bool add_token_word(struct lexer *lexer, char next)
{
    if (lexer->lexem->size == 0) // empty
        return false;

    // dup lexem
    char *lexem = vector_to_string(lexer->lexem);
    assert(lexem && lexem[0] != 0);
    vector_reset(lexer->lexem, 16);

    // transform to reserved word
    enum TokenType type = word_to_reserved_word(lexem);

    // manage alias
    if (type == LWORD && alias_is(lexem))
    {
        alias_substitution(lexer, lexem, next);
        free(lexem);
        return true;
    }

    // is it an assignment
    if (is_an_assignment(lexem))
        type = ASSIGNMENT;

    struct token *token = token_init(type, lexem);
    list_add(lexer->tokens, token);
    return false;
}

static bool add_token_potential_io(struct lexer *lexer, char next)
{
    if (lexer->lexem->size == 0)
        return false;

    if (add_token_word(lexer, next))
        return true;
    int i = list_size(lexer->tokens) - 1;
    if (i < 0)
        return false;
    struct token *token = list_get(lexer->tokens, i);

    if (is_number(token->lexem))
        token->type = IO_NUMBER;
    return false;
}

void add_token(struct lexer *lexer, enum TokenType type)
{
    assert(type != LWORD);

    struct token *token = token_init(type, NULL);
    list_add(lexer->tokens, token);
}

static void lex_hashtag(struct lexer *lexer)
{
    while (!is_at_end(lexer) && !lexer_check(lexer, '\n'))
    {
        advance(lexer);
    }
    if (add_token_word(lexer, '#'))
        return;
}

void lex_dollar(struct lexer *lexer)
{
    add_to_lexem(lexer, '$');
    if (lexer_match(lexer, '{'))
    {
        lex_dollar_bracket(lexer);
    }
    else if (lexer_match(lexer, '('))
    {
        lex_dollar_par(lexer);
    }
    else
    {
        lex_dollar_without_bracket(lexer);
    }
}

static void scan2(struct lexer *lexer, char c);

// if end of file return true
// else false
static void scan(struct lexer *lexer)
{
    if (is_at_end(lexer)) // 1
    {
        if (add_token_word(lexer, EOF))
        {
            lexer->need_to_read = true;
            return;
        }
        add_token(lexer, ENDOF);
        lexer->part_of_word = false;
        return;
    }

    char c = advance(lexer);
    // log_messp(lexer->log, "c = '%c'\n", c);

    if (c == '\n') // extra
    {
        if (add_token_word(lexer, c))
            return;
        add_token(lexer, NEWLINE);
        lexer->part_of_word = false;
    }
    else if (c == ')' || c == '(') // extra
    {
        if (add_token_word(lexer, c))
            return;
        add_token(lexer, c == '(' ? LPAR : RPAR);
        lexer->part_of_word = false;
    }
    else if (beginning_of_ope(c)) // 2 and 3 : manage operator
    {
        if (beginning_of_redir(c))
        {
            if (add_token_potential_io(lexer, c))
                return;
        }
        else
        {
            if (add_token_word(lexer, c))
                return;
        }
        manage_operator(lexer, c);
    }
    else if ((c == '\\' || c == '\'' || c == '"') && !lexer->quoted) // 4
    {
        if (c == '\\')
            lex_backslash(lexer);
        else if (c == '\'')
            lex_single_quote(lexer);
        else
            lex_double_quote(lexer);
    }
    else // 5, 6, 7, 8, 9, 10
        scan2(lexer, c);
}

static void scan2(struct lexer *lexer, char c) // It is for you Pierre <3
{
    if (!lexer->quoted && (c == '$' || c == '`')) // 5
    {
        if (c == '$')
            lex_dollar(lexer);
        else
            lex_backquote(lexer);
    }
    else if (!lexer->quoted && beginning_of_ope(c)) // 6
    {
        if (add_token_word(lexer, c))
            return;
        add_to_lexem(lexer, c);
    }
    else if (!lexer->quoted && is_blank(c)) // 7
    {
        if (add_token_word(lexer, c))
            return;
        lexer->part_of_word = false;
    }
    else if (lexer->part_of_word) // 8 : word += c
    {
        add_to_lexem(lexer, c);
    }
    else if (c == '#') // 9
    {
        lex_hashtag(lexer);
        lexer->part_of_word = false;
    }
    else // 10 : word = c
    {
        // the beginning of the word
        lexer->part_of_word = true;
        add_to_lexem(lexer, c);
    }
}

static void scan_tokens_ll1(struct lexer *lexer)
{
    while (list_empty(lexer->tokens))
    {
        scan(lexer);

        if (lexer->error != 0)
            errx(2, "Lexical error");
    }

    lexer_print(lexer);
}

void scan_tokens_ll2(struct lexer *lexer)
{
    while (list_size(lexer->tokens) <= 1
           && (list_empty(lexer->tokens)
               || ((struct token *)list_first(lexer->tokens))->type != ENDOF))
    {
        scan(lexer);
        if (lexer->error != 0)
            errx(2, "Lexical error");
    }

    lexer_print(lexer);
}

// DO NOT FORGET : this fonction destroy previous token
void lexer_move_forward(struct lexer *lexer)
{
    if (list_empty(lexer->tokens))
        return;

    struct token *token = list_pop_front(lexer->tokens);
    token_destroy(token);
}

static enum TokenType
get_token_type(struct token *token, bool must_or_could_be_reserved, bool is_ass)
{
    if (must_or_could_be_reserved || is_ass)
        return token->type;

    if (!is_reserved_type(token->type) && token->type != ASSIGNMENT)
        return token->type;

    // handle interpreting reserved or IO number word like simple word
    return LWORD;
}

static bool check_token(struct token *token, enum TokenType type_expected,
                        bool must_or_could_be_reserved)
{
    if (must_or_could_be_reserved || type_expected == ASSIGNMENT)
        return token->type == type_expected;

    if (type_expected != LWORD)
        return token->type == type_expected;

    // handle interpreting reserved or assignment word like simple word
    return token->type == LWORD || is_reserved_type(token->type)
        || token->type == ASSIGNMENT;
}

// reserved = true => we want reserved word
// found: return right token
// not found: return NULL
// reserved = false => rest
enum TokenType lexer_get_token_type(struct lexer *lexer,
                                    bool must_or_could_be_reserved, bool is_ass)
{
    if (list_empty(lexer->tokens))
        scan_tokens_ll1(lexer);

    return get_token_type(list_first(lexer->tokens), must_or_could_be_reserved,
                          is_ass);
}

bool lexer_check_token(struct lexer *lexer, enum TokenType type_expected,
                       bool must_or_could_be_reserved)
{
    if (list_empty(lexer->tokens))
        scan_tokens_ll1(lexer);

    return check_token(list_first(lexer->tokens), type_expected,
                       must_or_could_be_reserved);
}

char *lexer_get_token_lexem(struct lexer *lexer)
{
    struct token *token = list_first(lexer->tokens);
    assert(token != NULL);
    assert(token->type == IO_NUMBER || token->type == LWORD
           || token->type == ASSIGNMENT || is_reserved_type(token->type));

    return strdup(token->lexem);
}

enum TokenType lexer_get_token_type_ll2(struct lexer *lexer,
                                        bool must_or_could_be_reserved,
                                        bool is_ass)
{
    if (list_size(lexer->tokens) <= 1)
        scan_tokens_ll2(lexer);

    if (list_size(lexer->tokens) <= 1)
        return ENDOF;

    return get_token_type(list_get(lexer->tokens, 1), must_or_could_be_reserved,
                          is_ass);
}

bool lexer_check_token_ll2(struct lexer *lexer, enum TokenType type_expected,
                           bool must_or_could_be_reserved)
{
    if (list_size(lexer->tokens) <= 1)
        scan_tokens_ll2(lexer);

    if (list_size(lexer->tokens) <= 1)
        return false;

    return check_token(list_get(lexer->tokens, 1), type_expected,
                       must_or_could_be_reserved);
}
