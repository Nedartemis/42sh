#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"
#include "sh_instance.h"
#include "struct.h"

void my_exit(char *args[], struct output_builtin *ob)
{
    (void)ob;

    size_t size = 0;
    for (; args[size]; size++)
        continue;

    if (size > 1)
        errx(4, "exit usage : exit [n]\n");

    struct sh *sh = sh_instance_get();

    if (size == 1)
        sh->return_code = atoi(args[0]);

    sh->need_to_exit = true;
}
