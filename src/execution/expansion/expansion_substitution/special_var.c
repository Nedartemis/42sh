#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "list.h"
#include "null_array.h"
#include "ope_string.h"
#include "sh_instance.h"
#include "struct.h"
#include "tools.h"
#include "vector.h"

#define BUFFER_SIZE 64
#define PATH_MAX 1024

enum special
{
    AT = 0,
    STAR = 1,
    INTERROGATION = 2,
    DOLLAR = 3,
    HASH = 4,
    RANDOM = 5,
    UID = 6,
    OLDPWD = 7,
    PWD = 8,
    IFS = 9,
    DIGIT = 10,

    ENDOFARR = 11,
};

typedef void (*special_func)(struct list *return_list);

static const char *special_token_to_string[] = {
    [AT] = "@",        [STAR] = "*",        [INTERROGATION] = "?",
    [DOLLAR] = "$",    [HASH] = "#",        [RANDOM] = "RANDOM",
    [UID] = "UID",     [OLDPWD] = "OLDPWD", [PWD] = "PWD",
    [IFS] = "IFS",

    [ENDOFARR] = NULL,
};

static const char *env_var_to_string[] = { "UID", "PWD", "IFS", "OLDPWD",
                                           NULL };

bool is_env_var(const char *name)
{
    return array_string_in(env_var_to_string, name);
}

bool is_special_var(const char *name)
{
    if (is_number(name))
        return true;

    return array_string_in(special_token_to_string, name);
}

bool is_special_char(const char c)
{
    const char name[] = { c, 0 };
    return is_special_var(name);
}

static void at_substitute(struct list *return_list)
{
    struct sh *sh = sh_instance_get();
    for (int i = 0; i < sh->special_env->argc; i++)
    {
        char *word = strdup(sh->special_env->argv[i]);
        list_add(return_list, word);
    }
}

static void star_substitute(struct list *return_list)
{
    struct sh *sh = sh_instance_get();
    struct vector *ret_vec = vector_init(BUFFER_SIZE);

    for (int i = 0; i < sh->special_env->argc; i++)
    {
        char *arg = sh->special_env->argv[i];

        // Ignore if option
        if (arg[0] == '-')
            continue;
        // Ignore if file
        vector_concat_string(ret_vec, arg);
    }

    char *word = strndup(ret_vec->data, ret_vec->size);
    list_add(return_list, (void *)word);

    vector_destroy(ret_vec);
}

static void interrogation_substitute(struct list *return_list)
{
    struct sh *sh = sh_instance_get();
    char number[4];
    sprintf(number, "%d", sh->return_code);

    char *word = strdup(number);
    list_add(return_list, word);
}

static void hash_substitute(struct list *return_list)
{
    struct sh *sh = sh_instance_get();
    char number[5];
    sprintf(number, "%d", sh->special_env->argc);

    char *word = strdup(number);
    list_add(return_list, word);
}

static void random_substitute(struct list *return_list)
{
    srand(time(0) + clock());
    int r = rand();
    char number[20];
    sprintf(number, "%d", r);

    char *word = strdup(number);
    list_add(return_list, word);
}

static void uid_substitute(struct list *return_list)
{
    uid_t user_id = getuid();
    char number[11];
    sprintf(number, "%ld", (long int)user_id);

    char *word = strdup(number);
    list_add(return_list, word);
}

static void oldpwd_substitute(struct list *return_list)
{
    char *oldpwd = getenv("OLDPWD");
    if (!oldpwd)
        return;
    char *word = strdup(oldpwd);
    list_add(return_list, word);
}

static void pwd_substitute(struct list *return_list)
{
    char *pwd = getenv("PWD");
    char *word;
    if (!pwd)
    {
        char cwd[PATH_MAX];
        getcwd(cwd, PATH_MAX);
        word = strdup(cwd);
    }
    else
        word = strdup(pwd);
    list_add(return_list, word);
}

static void ifs_substitute(struct list *return_list)
{
    char *ifs_value = getenv("IFS");
    if (!ifs_value)
    {
        setenv("IFS", " \t\n", 0);
        ifs_value = getenv("IFS");
    }

    char *word = strdup(ifs_value);
    list_add(return_list, word);
}

static void dollar_substitute(struct list *return_list)
{
    pid_t pid = getpid();
    char number[11];
    sprintf(number, "%ld", (long int)pid);

    char *word = strdup(number);
    list_add(return_list, word);
}

static void digit_substitute(struct list *return_list, const char *name)
{
    struct sh *sh = sh_instance_get();
    int digit = atoi(name);

    if (digit < sh->argc)
    {
        char *word = strdup(sh->argv[digit]);
        list_add(return_list, word);
    }
}

static enum special find_special(const char *name)
{
    if (isdigit(name[0]))
        return 10;

    for (size_t i = 0; i < 10; i++)
    {
        const char *tok = special_token_to_string[i];
        if (strcmp(tok, name) == 0)
            return i;
    }

    // Should not reach this case
    assert(1 == 0);
    return -1;
}

static void call_special_func(const char *name, struct list *return_list)
{
    // find which special it is
    enum special special_tok = find_special(name);

    // Case special is digit
    if (special_tok == DIGIT)
    {
        digit_substitute(return_list, name);
        return;
    }

    // Other cases
    static const special_func functions[] = {
        [AT] = &at_substitute,
        [INTERROGATION] = &interrogation_substitute,
        [STAR] = &star_substitute,
        [HASH] = &hash_substitute,
        [RANDOM] = &random_substitute,
        [UID] = &uid_substitute,
        [OLDPWD] = &oldpwd_substitute,
        [PWD] = &pwd_substitute,
        [IFS] = &ifs_substitute,
        [DOLLAR] = &dollar_substitute
    };

    (*functions[special_tok])(return_list);
}

struct list *special_substitution(const char *name)
{
    // 1 --- Parcours list token and compare to name to get enum
    // 2 --- Appeler fonction de la liste

    // get list words from the right special var
    struct list *words = list_init();
    call_special_func(name, words);

    return words;
}
