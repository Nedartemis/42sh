#include <stdio.h>

#include "lexer.h"
#include "sh.h"

int main(int argc, char *argv[])
{
    // return lexer_test(argc, argv);
    return sh_main(argc, argv);
}
