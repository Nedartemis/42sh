#include <stdlib.h>
#include <string.h>

#include "builtin.h"
#include "dico_word.h"
#include "sh_instance.h"

void my_export(char *args[], struct output_builtin *ob)
{
    char *name;
    char *value;
    int return_code;
    struct sh *sh;

    return_code = 0;
    sh = sh_instance_get();

    for (int i = 0; args[i]; i++)
    {
        char *tmp = strdup(args[i]);
        name = strtok_r(tmp, "=", &value);

        // NO VALUES
        if (name == NULL)
            name = tmp;

        char *value_in_dico = (char *)dico_word_get(sh->dv, name);

        if (value[0] == '\0')
            value = value_in_dico;

        // setenv returns -1 on error, export returns the number of failed
        // export
        int set_env_err = setenv(name, value, 1);

        if (set_env_err != -1) // lets not add it if it didnt work
            dico_word_insert(sh->dv, name, value);

        return_code -= set_env_err;

        free(tmp);
    }

    ob->return_code = return_code;

    if (return_code != 0)
    {
        vector_concat_string(
            ob->error,
            "my_export : failed to export one or more of the given variables");
    }
}
