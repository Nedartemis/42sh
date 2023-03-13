#ifndef SIMPLE_COMMANDS_H
#define SIMPLE_COMMANDS_H

#include <sys/types.h>

#include "list.h"

int exec_command(char **argv);
int run_simple_command(char *words[]);

#endif /* !SIMPLE_COMMAND_H */
