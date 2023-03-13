#ifndef SH_H
#define SH_H

#include <stdbool.h>

#include "parser.h"
#include "struct.h"

// sh.c

struct sh *sh_init(int argc, char **argv);
void sh_reset(struct sh *sh);
void sh_copy_dico(struct sh *sh, struct dico_word *dv, struct dico_func *df);
int sh_run(struct sh *sh);
int sh_main(int argc, char *argv[]);
void sh_destroy(struct sh *sh);
int sh_run(struct sh *sh);

#endif /* ! SH_H */
