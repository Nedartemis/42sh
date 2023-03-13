#ifndef OPERATOR_H
#define OPERATOR_H

#include <stdbool.h>

#include "lexer.h"

// operator.c

bool beginning_of_ope(char c);
bool beginning_of_redir(char c);
void manage_operator(struct lexer *lexer, char c);

#endif /* !OPERATOR_H */
