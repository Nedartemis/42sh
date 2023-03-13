#ifndef AST_GRAPHVIZ_H
#define AST_GRAPHVIZ_H

#include <stdio.h>
#include <stdlib.h>

#include "ast.h"

void ast_cmd_write_graph(struct ast *ast, FILE *file, struct node_counters *nc);
void ast_if_write_graph(struct ast *ast, FILE *file, struct node_counters *nc);
void ast_list_write_graph(struct ast *ast, FILE *file,
                          struct node_counters *nc);
void ast_write_graph(struct ast *ast, FILE *file, struct node_counters *nc);
void ast_create_graph_file(struct ast *ast);
void ast_pipeline_write_graph(struct ast *ast, FILE *file,
                              struct node_counters *nc);
void ast_prefix_write_graph(struct ast *ast, FILE *file,
                            struct node_counters *nc);

#endif /* !AST_GRAPHVIZ_H */
