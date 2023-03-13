#include "parse_rule.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "ast2.h"
#include "ast_destroy.h"
#include "ast_struct.h"
#include "lexer_run.h"
#include "log.h"
#include "parse_rule2.h"
#include "parser.h"
#include "parser_helper.h"
#include "parser_helper2.h"

// input =
//    list '\n'
//  | list EOF
//  | '\n'
//  | EOF
//  ;

struct ast *parse_input(struct sh *sh)
{
    if (match(sh->parser, NEWLINE, false))
    {
        return NULL;
    }
    else if (match(sh->parser, ENDOF, false))
    {
        sh->eof = true;
        return NULL;
    }

    struct ast *ast = parse(sh, RLIST);

    if (!ast)
        goto not_generated;

    if (match(sh->parser, NEWLINE, false))
        return ast;

    if (match(sh->parser, ENDOF, false))
    {
        sh->eof = true;
        return ast;
    }

not_generated:
    print_not_generated("input");
    perror("Error syntax");
    ast_destroy(ast);
    sh->return_code = 2;
    return NULL;
}

// list = and_or { ';' and_or } [ ';' ] ;

struct ast *parse_list(struct sh *sh)
{
    struct list *asts = list_init();
    struct ast *res = ast_list_init(asts);

    // and_or
    struct ast *ast = parse(sh, RAND_OR);
    if (!ast)
        goto error_syntax;

    // adds the first ast_and_or
    list_add(asts, ast);

    // { ';' and_or } [ ';' ]
    while (match(sh->parser, SEMICOLON, false))
    {
        ast = parse(sh, RAND_OR);
        if (!ast)
            break;
        list_add(asts, ast);
    }

    return res;

error_syntax:
    print_not_generated("list");
    ast_destroy(res);
    return NULL;
}

// and_or = pipeline { '&&' | '||' {'\n'} pipeline } ;

struct ast *parse_and_or(struct sh *sh)
{
    struct ast *ast = ast_and_or_init(-1, NULL, NULL);
    struct ast_and_or *ast_and_or = (struct ast_and_or *)ast;

    // pipeline
    ast_and_or->left = parse(sh, RPIPELINE);

    if (!ast_and_or->left)
        goto not_generated;

    while (true)
    {
        // '&&' | '||'
        if (match(sh->parser, AND_IF, false))
            ast_and_or->type = AND_IF;
        else if (match(sh->parser, OR_IF, false))
            ast_and_or->type = OR_IF;
        else // case no ast_and_or->right
            return ast;

        // {'\n'}
        while (match(sh->parser, NEWLINE, false))
            continue;

        // pipeline
        struct ast *pipeline = parse(sh, RPIPELINE);
        if (!pipeline)
            goto error_syntax;

        ast_and_or->right = pipeline;
        ast = ast_and_or_init(-1, ast, NULL);
        ast_and_or = (struct ast_and_or *)ast;
    }

    return ast;

error_syntax:
    sh->return_code = 2;
    perror("Error syntax");
not_generated:
    print_not_generated("and_or");
    ast_destroy(ast);
    return NULL;
}

// pipeline = ['!'] command { '|' {'\n'} command } ;

struct ast *parse_pipeline(struct sh *sh)
{
    // ast_pipeline init
    struct ast *pipeline = ast_pipeline_init(NULL, NULL);
    struct ast_pipeline *ast_pipeline = (struct ast_pipeline *)pipeline;

    // ['!']
    if (match(sh->parser, BANG, true))
        ast_pipeline->istrue = false;

    // command
    struct ast *left = parse(sh, RCOMMAND); // left command
    if (!left)
        goto not_generated;

    // adds cmd to ast_pipeline->left
    ast_pipeline->left = left;

    // { '|' {'\n'} command }
    if (match(sh->parser, PIPE, false)) // '|'
    {
        // {'\n'}
        while (match(sh->parser, NEWLINE, false))
            continue;

        // check that there is no '!'
        if (check(sh->parser, BANG, true))
            goto not_generated;

        // command
        struct ast *right = parse(sh, RPIPELINE);

        // error
        if (!right)
            goto error_syntax;

        // adds pipeline to ast_pipeline->right
        ast_pipeline->right = right;
    }

    return pipeline;

error_syntax:
    sh->return_code = 2;
    perror("Error syntax");
not_generated:
    print_not_generated("pipeline");
    ast_destroy(pipeline);
    return NULL;
}

// command =
//    simple_command
//  | shell_command { redirection }
//  | funcdec { redirection }
//  ;

// returns an ast_list
// case simple_command : the command is the first element
// case shell_command : the command is the first element
//                      and the next elements are redirs

struct ast *parse_command(struct sh *sh)
{
    // funcdec
    struct ast *ast = parse(sh, RFUNCDEC);
    if (ast) // { redirection }
    {
        assert(ast->type == AST_FUNC);
        struct ast_func *ast_func = (struct ast_func *)ast;
        ast_func->body = parse_redirect(sh, ast_func->body);
        return ast;
    }

    // in no funcdec, try simple_command
    ast = parse(sh, RSIMPLE_COMMAND);
    if (ast)
        return ast;

    // if no simple_command found: try shell_command

    // shell_command
    ast = parse(sh, RSHELL_COMMAND);

    // no funcdec nor simple_command nor shell_command
    if (!ast)
        goto not_generated;

    // { redirection }
    return parse_redirect(sh, ast);

not_generated:
    ast_destroy(ast);
    print_not_generated("command");
    return NULL;
}

// simple_command =
//    prefix { prefix }
//  | { prefix } WORD { element }
//  ;

static void simple_command_add_element(struct sh *sh, struct ast_cmd *ast_cmd,
                                       struct list *redirs)
{
    for (; true;)
    {
        struct ast *ast_element = parse(sh, RELEMENT);
        if (!ast_element)
            break;

        // assert ast is ast_element
        if (ast_element->type == AST_ELEMENT)
        {
            // add word from ast_element
            struct ast_element *element = (struct ast_element *)ast_element;
            list_add(ast_cmd->words, strdup(element->word));
            ast_element->vtable->destroy(&element->base);
        }
        else // case redir
        {
            list_prepend(redirs, ast_element);
        }
    }
}

struct ast *parse_simple_command(struct sh *sh)
{
    struct ast *ast = ast_cmd_init(list_init(), list_init());
    struct ast_cmd *ast_cmd = (struct ast_cmd *)ast;

    // storages ast_redir
    struct list *redirs = list_init();

    // prefix
    struct ast *prefix = parse(sh, RPREFIX);

    //  { prefix }
    while (prefix)
    {
        // case redirection
        if (prefix->type == AST_REDIR)
            list_prepend(redirs, prefix);
        else
        {
            assert(prefix->type == AST_PREFIX);
            list_add(ast_cmd->ast_prefixes, prefix);
        }
        prefix = parse(sh, RPREFIX);
    }

    // word
    if (!check(sh->parser, LWORD, true))
    {
        if (list_empty(ast_cmd->ast_prefixes) && list_empty(redirs))
            goto not_generated;

        // generated prefix { prefix }
        goto create_redirs;
    }

    // case { prefix } WORD { element }
    char *word = eat_word(sh->parser);

    // add first asts
    list_add(ast_cmd->words, word);

    // add list element
    simple_command_add_element(sh, ast_cmd, redirs);

create_redirs:
    while (!list_empty(redirs))
    {
        struct ast *redir = list_pop_front(redirs);
        assert(redir != NULL);
        struct ast_redir *ast_redir = (struct ast_redir *)redir;
        ast_redir->cmd = ast;
        ast = redir;
    }

    list_destroy(redirs, destroy_ast_void);
    // return ast
    print_tabs("stops correctly", sh->parser->nbrec);
    return ast;

not_generated:
    print_not_generated("simple_command");
    list_destroy(redirs, destroy_ast_void);
    ast_destroy(ast);
    return NULL;
}

// shell_command =
//    '(' compound_list ')' // = subshell
//  | '{' compound_list '}'
//  | rule_if
//  | rule_while
//  | rule_until
//  | rule_case
//  | rule_for
//  ;

struct ast *parse_shell_command(struct sh *sh)
{
    struct ast *ast;

    // case subshell
    // '('
    if (match(sh->parser, LPAR, true))
    {
        // compound_list
        ast = parse(sh, RCOMPOUND_LIST);

        // ')'
        if (!ast || !match(sh->parser, RPAR, true))
            goto error_syntax;

        return ast_subshell_init(ast);
    }

    // '{'
    if (match(sh->parser, LBRACKET, true))
    {
        // compound_list
        ast = parse(sh, RCOMPOUND_LIST);

        // '}'
        if (!ast || !match(sh->parser, RBRACKET, true))
            goto error_syntax;

        return ast;
    }

    // rule_if
    ast = parse(sh, RRULE_IF);
    if (ast)
        return ast;

    // rule_case
    ast = parse(sh, RCASE);
    if (ast)
        return ast;

    // rule_while | rule_until
    ast = parse(sh, RWHILE);
    if (ast)
        return ast;

    // rule_for
    ast = parse(sh, RFOR);
    if (!ast)
        goto not_generated;

    return ast;

error_syntax:
    sh->return_code = 2;
    perror("Error syntax");
not_generated:
    print_not_generated("shell_command");
    ast_destroy(ast);
    return NULL;
}

// rule_if = 'if' compound_list 'then' compound_list [else_clause] 'fi' ;

struct ast *parse_rule_if(struct sh *sh)
{
    struct ast_if *ast_if = (struct ast_if *)ast_if_init(NULL, NULL, NULL);

    // 'if'
    if (!match(sh->parser, IF, true))
        goto not_generated;

    // compound_list 'then'
    ast_if->conditions = parse(sh, RCOMPOUND_LIST);
    if (!ast_if->conditions || !match(sh->parser, THEN, true))
        goto error_syntax;

    // compound_list
    ast_if->if_body = parse(sh, RCOMPOUND_LIST);
    if (!ast_if->if_body)
        goto error_syntax;

    // [else_clause]
    ast_if->else_body = parse(sh, RELSE_CLAUSE);

    // 'fi'
    if (!match(sh->parser, FI, true))
        goto error_syntax;

    return &ast_if->base;

error_syntax:
    sh->return_code = 2;
    perror("Error syntax");
not_generated:
    if (ast_if)
        ast_destroy(&ast_if->base);
    print_not_generated("rule_if");
    return NULL;
}

// else_clause =
//    'else' compound_list
//  | 'elif' compound_list 'then' compound_list [else_clause]
//  ;

struct ast *parse_else_clause(struct sh *sh)
{
    struct ast_if *ast_if = NULL;

    // 'else'
    if (match(sh->parser, ELSE, true))
    {
        return parse(sh, RCOMPOUND_LIST);
    }
    else if (!match(sh->parser, ELIF, true)) // 'elif'
    {
        goto not_generated;
    }

    ast_if = (struct ast_if *)ast_if_init(NULL, NULL, NULL);

    // compound_list 'then'
    ast_if->conditions = parse(sh, RCOMPOUND_LIST);
    if (!ast_if->conditions || !match(sh->parser, THEN, true))
        goto error_syntax;

    // compound_list
    ast_if->if_body = parse(sh, RCOMPOUND_LIST);
    if (!ast_if->if_body)
        goto error_syntax;

    // [else_clause]
    ast_if->else_body = parse(sh, RELSE_CLAUSE);

    // 'fi'
    if (!match(sh->parser, FI, true))
        goto error_syntax;

    return &ast_if->base;

error_syntax:
    sh->return_code = 2;
    perror("Error syntax");
not_generated:
    if (ast_if)
        ast_destroy(&ast_if->base);
    print_not_generated("else_clause");
    return NULL;
}
