#include "lexer.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

#include "lexer_run.h"
#include "list.h"
#include "token.h"

struct lexer *lexer_init(bool log)
{
    // allocate memory
    struct lexer *lexer = calloc(1, sizeof(struct lexer));
    if (!lexer)
        errx(3, "lexer_init: Not enough memory");

    // init atributs
    lexer->lexem = vector_init(16);
    lexer->current = 0;
    lexer->tokens = list_init();
    lexer->log = log;
    lexer->need_to_read = true;
    lexer->part_of_word = false;
    lexer->quoted = false;
    lexer->error = 0;
    lexer->annexe_input = NULL;

    return lexer;
}

void lexer_reset(struct lexer *lexer)
{
    lexer->need_to_read = true;
    lexer->part_of_word = false;
    lexer->quoted = false;
    lexer->error = 0;
    lexer->current = 0;

    assert(list_empty(lexer->tokens));
    assert(lexer->lexem->size == 0);
}

void lexer_give_input(struct lexer *lexer, FILE *input)
{
    lexer->input = input;
}

void lexer_print(struct lexer *lexer)
{
    if (!lexer->log)
        return;

    printf("Lexer :\n");
    printf("Nb token : %ld\n", list_size(lexer->tokens));
    list_print(lexer->tokens, token_print_data);
    printf("\n");
}

void lexer_destroy(struct lexer *lexer)
{
    if (lexer->input)
        fclose(lexer->input);

    // destroy attributs expect input
    list_destroy(lexer->tokens, token_destroy_data);
    vector_destroy(lexer->lexem);

    // destroy itself
    free(lexer);
}
