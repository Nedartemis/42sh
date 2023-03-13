#include <stdlib.h>
#include <string.h>

#include "ast_destroy.h"
#include "builtin.h"
#include "dico_word.h"
#include "expansion.h"
#include "expansion_helper.h"
#include "hash_map.h"
#include "sh.h"
#include "sh_instance.h"
#include "struct.h"

static void assign_alias(struct sh *sh, char *arg, struct output_builtin *ob);
static void unassign_alias(struct sh *sh, char *name,
                           struct output_builtin *ob);
static void unassign_alias_all(struct sh *sh);
static void print_alias_all(struct sh *sh, struct output_builtin *ob);
static void print_alias(struct sh *sh, char *name, struct output_builtin *ob);

// args[] = { "foo=bar" , "foo" , "truc=machin" }
void my_alias(char *args[], struct output_builtin *ob)
{
    struct sh *sh = sh_instance_get();
    // alias
    if (args[0] == NULL)
    {
        print_alias_all(sh, ob);
        return;
    }

    // alias { ... }
    for (size_t i = 0; args[i] != NULL; i++)
        assign_alias(sh, args[i], ob);
}

void my_unalias(char *args[], struct output_builtin *ob)
{
    if (!args)
        return;

    struct sh *sh = sh_instance_get();

    // unalias -a
    if (args[0] && !args[1] && strcmp(args[0], "-a") == 0)
    {
        unassign_alias_all(sh);
        return;
    }

    // unalias { alias_name }
    for (size_t i = 0; args[i] != NULL; i++)
        unassign_alias(sh, args[i], ob);
}

static void assign_alias(struct sh *sh, char *arg, struct output_builtin *ob)
{
    // split name and value if there is
    size_t i = 0;
    for (; arg[i] != '='; i++)
    {
        // case no '='
        if (arg[i] == '\0')
        {
            print_alias(sh, arg, ob);
            return;
        }
        continue;
    }

    char *name = strndup(arg, i);
    char *value = strdup(arg + i + 1);

    char *value_expanded = concat_list_string(expansion(value, false), ' ');

    dico_word_insert(sh->da, name, value_expanded);

    free(name);
    free(value);
    free(value_expanded);
}

static void unassign_alias(struct sh *sh, char *name, struct output_builtin *ob)
{
    if (!dico_word_in(sh->da, name))
    {
        ob->return_code = 1;
        perror("unalias: not found");
        return;
    }

    dico_word_remove(sh->da, name);
}

static void unassign_alias_all(struct sh *sh)
{
    dico_word_destroy(sh->da);
    sh->da = dico_word_init();
}

static void print_alias_all(struct sh *sh, struct output_builtin *ob)
{
    struct list *sorted = hash_map_sort(sh->da->hm);
    size_t len = list_size(sorted);

    for (size_t i = 0; i < len; i++)
    {
        char *name = list_get(sorted, i);
        print_alias(sh, name, ob);
    }
    list_destroy(sorted, destroy_word_cmd);
}

static void print_alias(struct sh *sh, char *name, struct output_builtin *ob)
{
    if (!dico_word_in(sh->da, name))
    {
        ob->return_code = 1;
        perror("alias: not found");
        return;
    }
    printf("%s=\'%s\'\n", name, dico_word_get(sh->da, name));
}
