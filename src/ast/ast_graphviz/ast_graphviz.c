#include "ast_graphviz.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ast.h"
#include "ast_destroy.h"
#include "ast_struct.h"
#include "list.h"

// USING GRAPHVIZ

void printer(char *type, FILE *file, size_t nc_type, char *word)
{
    if (!word)
    {
        fprintf(file, "\t\"%s n.%ld\" -> ", type, nc_type);
    }
    else
    {
        fprintf(file, "\t\"%s n.%ld\" -> \"%s\";\n", type, nc_type, word);
    }
}

void ast_cmd_write_graph(struct ast *ast, FILE *file, struct node_counters *nc)
{
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;
    size_t size = list_size(ast_cmd->words);
    for (size_t i = 0; i < size; i++)
    {
        char *word = list_get(ast_cmd->words, i);
        printer("CMD", file, nc->nc_cmd, word);
    }
    nc->nc_cmd++;
}

void ast_if_write_graph(struct ast *ast, FILE *file, struct node_counters *nc)
{
    struct ast_if *ast_if = (struct ast_if *)ast;
    printer("IF", file, nc->nc_if, NULL);
    ast_write_graph(ast_if->conditions, file, nc);
    printer("IF", file, nc->nc_if, NULL);
    if (!ast_if->else_body)
        nc->nc_if++;
    ast_write_graph(ast_if->if_body, file, nc);
    if (ast_if->else_body)
    {
        printer("IF", file, nc->nc_if, NULL);
        nc->nc_if++;
        ast_write_graph(ast_if->else_body, file, nc);
    }
    nc->nc_cmd++;
}

void ast_list_write_graph(struct ast *ast, FILE *file, struct node_counters *nc)
{
    struct ast_list *ast_list = (struct ast_list *)ast;
    size_t size = list_size(ast_list->asts);
    nc->nc_list++;
    for (size_t i = 0; i < size; i++)
    {
        printer("LIST", file, nc->nc_list, NULL);
        ast_write_graph(list_get(ast_list->asts, i), file, nc);
    }
}

void ast_pipeline_write_graph(struct ast *ast, FILE *file,
                              struct node_counters *nc)
{
    struct ast_pipeline *ast_pipeline = (struct ast_pipeline *)ast;
    struct ast *ast_left = ast_pipeline->left;
    struct ast *ast_right = ast_pipeline->right;
    nc->nc_pipeline++;

    printer("PIPE", file, nc->nc_pipeline, NULL);
    ast_write_graph(ast_left, file, nc);
    if (ast_right)
    {
        printer("PIPE", file, nc->nc_pipeline, NULL);
        ast_write_graph(ast_right, file, nc);
    }
}

void ast_prefix_write_graph(struct ast *ast, FILE *file,
                            struct node_counters *nc)
{
    struct ast_prefix *ast_prefix = (struct ast_prefix *)ast;
    printer("PREFIX", file, nc->nc_prefix, ast_prefix->ass_word);
    nc->nc_prefix++;
}

void ast_write_graph(struct ast *ast, FILE *file, struct node_counters *nc)
{
    if (!ast)
        return;
    ast->vtable->graph_viz(ast, file, nc);
}

void ast_create_graph_file(struct ast *ast)
{
    FILE *file = fopen("graph_viz.dot", "w+");
    fprintf(file, "digraph {\n");
    if (!file)
        return;
    else
    {
        struct node_counters *nc = malloc(sizeof(struct node_counters));
        nc->nc_cmd = 0;
        nc->nc_words = 0;
        nc->nc_pipeline = 0;
        nc->nc_if = 0;
        nc->nc_list = 0;
        ast_write_graph(ast, file, nc);
        fprintf(file, "}");
        fclose(file);
        free(nc);
    }

    // system("dot -Tps graph_viz.dot -o graph_viz.ps");
    //  system("evince graph_viz.ps");
}
