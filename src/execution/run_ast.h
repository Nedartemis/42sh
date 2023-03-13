#ifndef RUN_AST_H
#define RUN_AST_H

#include "ast.h"
#include "ast_struct.h"
#include "struct.h"

int run_ast(struct ast *ast); // head function to run the entire ast
int run_if(struct ast *ast); // run function FOR AST_IF
int run_list(struct ast *ast); // run function for AST_LIST
int run_cmd(struct ast *ast); // run function for AST_CMD
int run_redir(struct ast *ast);
int run_pipeline(struct ast *ast); // run function for AST_PIPE
int run_and_or(struct ast *ast);
int run_while(struct ast *ast);
int run_for(struct ast *ast);
int run_prefix(struct ast *ast);

#endif /* !RUN_AST_H */
