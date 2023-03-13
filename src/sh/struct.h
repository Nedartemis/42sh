#ifndef STRUCT_H
#define STRUCT_H

#include <stdbool.h>

#include "ast_struct.h"
#include "list.h"

struct parser
{
    struct lexer *lexer;
    int nbrec;
    bool log;
};

enum BuiltinType
{
    CONTINUE,
    BREAK,
    NOTHING,
};

struct special_env
{
    int argc;
    char **argv;
};

struct sh
{
    bool log;
    int argc;
    char **argv;
    bool eof;
    int return_code;

    // variables @ * #
    struct special_env *special_env;

    // struct
    struct parser *parser;
    struct ast *root_ast;

    // dico
    struct dico_word *dv;
    struct dico_func *df;
    struct dico_word *da;

    // func
    void (*destroy)(struct sh *);

    // for builtin
    int nbloop;
    int nbtomanage;
    enum BuiltinType builtin_type;
    bool need_to_exit;
};

#endif /* !STRUCT_H */
