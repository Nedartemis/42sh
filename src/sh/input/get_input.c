#include "get_input.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "log.h"

#define SIZE_BUFFER 1024

enum usage
{
    U_STRING = 0,
    U_FILE = 1,
    U_STANDART = 2
};

static enum usage find_usage(int argc, char **argv, struct sh *sh)
{
    // CASE STANDART
    if (argc == 1)
    {
        sh->special_env->argc = argc - 1;
        sh->special_env->argv = argv + 1;
        return U_STANDART;
    }

    // CASE FILE
    if (strcmp(argv[1], "-c") != 0)
    {
        if (access(argv[1], F_OK) != 0)
        {
            perror("Error : File not found");
            errx(2, "Usage : 42sh [OPTIONS] [SCRIPT] [ARGUMENTS ...]");
        }
        else
        {
            sh->special_env->argc = argc - 2;
            sh->special_env->argv = argv + 2;
            return U_FILE;
        }
    }
    else
    {
        sh->special_env->argc = argc - 3;
        sh->special_env->argv = argv + 3;
        return U_STRING;
    }
}

// Verify its a file and returns struct input
FILE *get_input(struct sh *sh)
{
    int argc = sh->argc;
    char **argv = sh->argv;

    log_mess(sh, "----------- Get input --------------\n");

    // get tusage
    enum usage usage = find_usage(argc, argv, sh);

    // init input
    FILE *input = NULL;

    size_t size;

    // Case file
    switch (usage)
    {
    case U_FILE:
        input = fopen(argv[1], "r");
        break;
    case U_STANDART:
        input = stdin;
        break;
    default: // Case string
        size = strlen(argv[2]);
        input = fmemopen(argv[2], size, "r");
        break;
    }

    return input;
}

void reset_input(struct sh *sh)
{
    enum usage usage = find_usage(sh->argc, sh->argv, sh);

    if (usage == U_STANDART)
        return;

    log_mess(sh, "----------- Reset input --------------\n");

    struct lexer *lexer = sh->parser->lexer;
    assert(lexer->input != NULL);

    int position = ftell(lexer->input);
    fclose(lexer->input);

    lexer->input = get_input(sh);
    fseek(lexer->input, position, SEEK_SET);
    log_mess(sh, "position = %d ; real position = %d\n", position,
             ftell(lexer->input));
}
