#include "functions.h"

#include "dico_func.h"
#include "run_ast.h"
#include "struct.h"

int run_function(struct dico_func *df, const char *name)
{
    int return_code;

    return_code = run_ast(dico_func_get(df, name));

    return return_code;
}
