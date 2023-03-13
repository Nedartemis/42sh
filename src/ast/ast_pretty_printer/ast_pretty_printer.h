#ifndef PRETTY_PRINTER_H
#define PRETTY_PRINTER_H

#include "ast.h"
#include "ast_struct.h"
#include "list.h"

// handle tabs
void pretty_print_tabs(void);
void change_tabs(int i);

// utilities
void print_word(void *data);

int func_test(void);
void ast_pretty_print(struct ast *ast);
void ast_cmd_pretty_print(struct ast *ast);
void ast_if_pretty_print(struct ast *ast);
void ast_list_pretty_print(struct ast *ast);
void element_pretty_print(struct ast *ast);
void ast_pipeline_pretty_print(struct ast *ast);

#endif /* !PRETTY_PRINTER_H */
