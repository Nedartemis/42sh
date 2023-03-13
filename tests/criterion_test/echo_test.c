#include <criterion/criterion.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "builtin.h"
#include "tools.h"

void expect_null(char *actual)
{
    if (!actual || actual[0] == '\0')
        return;

    cr_expect_null(actual, "Expected (null) ; Got (%s)", actual);
}

void expect_str(char *expected, char *actual)
{
    if (expected)
        cr_expect_str_eq(expected, actual, "Expected (%s) ; Got (%s)", expected,
                         actual);
    else
        expect_null(actual);
}

void expect_int(int expected, int actual)
{
    cr_expect_eq(expected, actual, "Expected (%d) ; Got (%d)", expected,
                 actual);
}

// -------------------- Test echo ------------------

void expect_echo(char *args[], int return_code, char *outpt, char *error)
{
    struct output_builtin *ob = output_builtin_init();

    my_echo(args, ob);

    expect_int(return_code, ob->return_code);
    expect_str(outpt, ob->outpt->data);
    if (!error)
        expect_null(ob->error->data);

    output_builtin_destroy(ob);
}

// ---------------------- Simple ----------------------

Test(echo_test_suite, echo_simple)
{
    char *args[] = { "hello world", NULL };
    expect_echo(args, 0, "hello world\n", NULL);
}

// ---------------------- -n ----------------------

Test(echo_test_suite, echo_n)
{
    char *args[] = { "-n", "hello world", NULL };
    expect_echo(args, 0, "hello world", NULL);
}

// ---------------------- Normal \n \t \\ ----------------------

Test(echo_test_suite, echo_backslash_n)
{
    char *args[] = { "hello \n world", NULL };
    expect_echo(args, 0, "hello \\n world\n", NULL);
}

Test(echo_test_suite, echo_tab)
{
    char *args[] = { "hello \t world", NULL };
    expect_echo(args, 0, "hello \\t world\n", NULL);
}

Test(echo_test_suite, echo_backslash_backslash)
{
    char *args[] = { "hello \\ world", NULL };
    expect_echo(args, 0, "hello \\\\ world\n", NULL);
}

// ---------------------- -E \n \t \\ ----------------------

Test(echo_test_suite, echo_E_backslash_n)
{
    char *args[] = { "-E", "hello \n world", NULL };
    expect_echo(args, 0, "hello \\n world\n", NULL);
}

Test(echo_test_suite, echo_E_tab)
{
    char *args[] = { "-E", "hello \t world", NULL };
    expect_echo(args, 0, "hello \\t world\n", NULL);
}

Test(echo_test_suite, echo_E_backslash_backslash)
{
    char *args[] = { "-E", "hello \\ world", NULL };
    expect_echo(args, 0, "hello \\\\ world\n", NULL);
}

// ---------------------- -e \n \t \\ ----------------------

Test(echo_test_suite, echo_e_backslash_n)
{
    char *args[] = { "-e", "hello \n world", NULL };
    expect_echo(args, 0, "hello \n world\n", NULL);
}

Test(echo_test_suite, echo_e_tab)
{
    char *args[] = { "-e", "hello \t world", NULL };
    expect_echo(args, 0, "hello \t world\n", NULL);
}

Test(echo_test_suite, echo_e_backslash_backslash)
{
    char *args[] = { "-e", "hello \\ world", NULL };
    expect_echo(args, 0, "hello \\ world\n", NULL);
}

// ---------------------- Wrong option but not Error ----------------------

Test(echo_test_suite, echo_wrong_option)
{
    char *args[] = { "-R", NULL };
    expect_echo(args, 0, "-R\n", NULL);
}
