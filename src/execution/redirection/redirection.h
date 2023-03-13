#ifndef REDIRECTION_H
#define REDIRECTION_H

#include "ast.h"

typedef int (*do_redirection)(int *io_number, char *word, bool io_default);

int redirect(struct ast_redir *red);
bool is_closing_op(char *word);
int my_atoi(char *word);
int word_to_fd(char *word, int open_flags);

#endif /* ! REDIRECTION_H */
