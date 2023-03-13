#include <assert.h>
#include <err.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "lexer.h"
#include "lexer_cmd_subst.h"
#include "list.h"
#include "ope_string.h"
#include "sh.h"
#include "sh_instance.h"
#include "vector.h"

enum pipe
{
    LEFT = 0,
    RIGHT = 1
};

// call sh_main(argc, argv) in a child process
static pid_t exec_fork_sh(int fds[2], int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid < 0)
        errx(3, "exec_fork_pipe : fork didn't work");
    if (pid != 0)
        return pid;

    // in the child
    // close the reading side of the pipe
    if (close(fds[LEFT]) == -1)
        errx(3, "exec_fork_sh : fail to close left side of the pipe");

    // copy the writing side of the pipe for the stdout and close it
    if (dup2(fds[RIGHT], STDOUT_FILENO) == -1)
        errx(3, "exec_fork_sh : dup2 case LEFT didn't work");
    close(fds[RIGHT]);

    // run the sh_run
    struct sh *sh = sh_instance_get();
    struct sh *new_sh = sh_init(argc, argv);
    sh_copy_dico(new_sh, sh->dv, sh->df);
    int return_code = sh_run(new_sh);
    sh_destroy(new_sh);

    // write eof
    printf("%c", '\0');
    exit(return_code);
}

static char *read_stdin(int fds[2])
{
    // close the writing side of the pipe
    if (close(fds[RIGHT]) == -1)
        errx(13, "read_stdin : fail to close left side of the pipe");

    // copy the stdin
    int copy_stdin = dup(STDIN_FILENO);
    if (copy_stdin == -1)
        errx(13, "read_stdin : fail dup stdin\n");

    // copy the reading side of the pipe for the stdin and close it
    if (dup2(fds[LEFT], STDIN_FILENO) == -1)
        errx(13, "read_stdin : dup2 case RIGHT didn't work");
    close(fds[LEFT]);

    // read stdi, put it into a vector and convert it to a string
    struct vector *vector = vector_init(16);
    char c;
    while ((c = fgetc(stdin)))
    {
        vector_append(vector, c);
    }
    char *res = vector_to_string(vector);
    vector_destroy(vector);

    // dup2 stdin
    if (dup2(copy_stdin, STDIN_FILENO) == -1)
        errx(13, "read_stdin : fail to redup stdin\n");
    if (close(copy_stdin) == -1)
        errx(13, "read_stdin : fail to close the copy of stdin\n");

    return res;
}

static int exec_pipe(int argc, char *argv[], char **out)
{
    // create the pipe
    int fds[2];
    if (pipe(fds) == -1)
        errx(1, "exec_pipe : fail to create pipe");

    // call the sh_main by a fork
    pid_t pid_left = exec_fork_sh(fds, argc, argv);
    // read the output of the sh_main and put into a string
    *out = read_stdin(fds);

    // wait the end of the sh_main
    int status;
    waitpid(pid_left, &status, 0);
    int return_code = WEXITSTATUS(status);

    return return_code;
}

static char *get_the_string_to_exec(char *input, size_t *i,
                                    void (*lex_func)(struct lexer *))
{
    // delimit the command substitution thanks to the lexer
    // init lexer
    struct lexer *lexer = lexer_init(false);
    // create and give input
    FILE *input_file = fmemopen(input + *i, strlen(input + *i), "r");
    if (!input_file)
        errx(5, "expansion_cmd_dollar : error to fmemopen");
    lexer_give_input(lexer, input_file);
    // lex the dollar par
    lex_func(lexer);

    // remove '('....')' from the result
    assert(lexer->lexem);
    ;
    char *cut = vector_to_string_begin(lexer->lexem, 1, lexer->lexem->size - 2);

    // advance (we already pass the openning par '(')
    *i += lexer->lexem->size - 1;

    // destroy
    lexer_destroy(lexer);

    return cut;
}

// as precise by the SCL for command substitution,
// the ending newline need to be cut
static void cut_new_line(char **str)
{
    size_t size = strlen(*str);
    if (size == 0)
        return;
    for (; (*str)[size - 1] == '\n'; size--)
        continue;

    char *new = strndup(*str, size);
    free(*str);
    *str = new;
}

static char *expansion_cmd_subst(char *input, size_t *i,
                                 void (*lex_func)(struct lexer *))
{
    char *str_to_exec = get_the_string_to_exec(input, i, lex_func);

    // construct argc and argv : Usage -c 'string'
    int argc = 3;
    char *argv[3] = { "./42sh", "-c", str_to_exec };

    // call the sh_main
    char *output = NULL;
    struct sh *sh = sh_instance_get();
    sh->return_code = exec_pipe(argc, argv, &output);

    free(str_to_exec);

    cut_new_line(&output);

    return output;
}

struct list *expansion_dollar_par(char *input, size_t *i)
{
    assert(input[*i] == '$');
    *i += 1;
    assert(input[*i] == '(');
    *i += 1;

    char *output = expansion_cmd_subst(input, i, lex_dollar_par);

    // add output in words
    struct list *words = list_init();
    list_prepend(words, output);

    return words;
}

struct list *expansion_backquote(char *input, size_t *i)
{
    assert(input[*i] == '`');
    *i += 1;

    char *output = expansion_cmd_subst(input, i, lex_backquote);

    struct list *words = list_init();
    list_prepend(words, output);

    return words;
}
