#include <err.h>
#include <list.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"
#include "lexer_run.h"
#include "token.h"

int lexer_test(int argc, char *argv[])
{
    if (argc != 2)
        errx(3, "Usage : command");

    struct lexer *lexer = lexer_init(true);
    FILE *input = fmemopen(argv[1], strlen(argv[1]), "r");
    lexer_give_input(lexer, input);

    while (true)
    {
        enum TokenType type = lexer_get_token_type(lexer, false, false);
        printf("type ll1 = ");
        token_print_type(type);
        printf("\n");
        enum TokenType type2 = lexer_get_token_type_ll2(lexer, false, false);
        printf("type ll2 = ");
        token_print_type(type2);
        printf("\n");
        if (type == ENDOF)
            break;

        lexer_move_forward(lexer);
    }

    lexer_destroy(lexer);

    return 0;
}
