#include <assert.h>
#include <err.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "dico_word.h"
#include "lexer.h"
#include "sh.h"
#include "sh_instance.h"
#include "string.h"

bool alias_is(const char *key)
{
    assert(key);
    return dico_word_in(sh_instance_get()->da, key);
}

void alias_substitution(struct lexer *lexer, const char *key, char next)
{
    struct sh *sh = sh_instance_get();

    assert(dico_word_in(sh->da, key));

    const char *value = dico_word_get(sh->da, key);

    size_t size = strlen(value) + 2;
    lexer->annexe_str = calloc(size, sizeof(char));
    memcpy(lexer->annexe_str, value, size - 2);
    lexer->annexe_str[size - 2] = next;
    lexer->annexe_str[size - 1] = '\0';

    // printf("lexer->annexe_str = '%s'\n", lexer->annexe_str);
    // printf("size = %ld\n", size);

    lexer->annexe_input = fmemopen(lexer->annexe_str, size - 1, "r");
    if (!lexer->annexe_input)
        errx(26, "alias_substitution : error to open annexe input\n");
}
