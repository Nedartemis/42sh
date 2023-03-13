#include "parse_rule2.h"

#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "ast2.h"
#include "ast_destroy.h"
#include "ast_struct.h"
#include "lexer_run.h"
#include "log.h"
#include "parse_rule.h"
#include "parser.h"
#include "parser_helper.h"
#include "parser_helper2.h"

// compound_list = {'\n'} and_or { ';' | '\n' {'\n'} and_or } [';'] {'\n'} ;

struct ast *parse_compound_list(struct sh *sh)
{
    struct list *asts = list_init();
    struct ast *ast = ast_list_init(asts);

    // {'\n'}
    while (match(sh->parser, NEWLINE, false))
        continue;

    // and_or
    struct ast *ast_and_or = parse(sh, RAND_OR);

    if (!ast_and_or)
        goto not_generated;

    // add and_or
    list_add(asts, ast_and_or);

    // { ';' | '\n' {'\n'} and_or } [';'] {'\n'}

    // ';' | '\n'
    while (match(sh->parser, SEMICOLON, false)
           || match(sh->parser, NEWLINE, false))
    {
        // {'\n'}
        while (match(sh->parser, NEWLINE, false))
            continue;

        // and_or
        ast_and_or = parse(sh, RAND_OR);

        if (!ast_and_or)
            return ast;

        // adds and_or
        list_add(asts, ast_and_or);
    }

    // [';']
    match(sh->parser, SEMICOLON, false);

    // {'\n'}
    while (match(sh->parser, NEWLINE, false))
        continue;

    return ast;

not_generated:
    ast_destroy(ast);
    print_not_generated("compound_list");
    return NULL;
}

// prefix =
//    ASSIGNMENT_WORD
//  | redir
//  ;

struct ast *parse_prefix(struct sh *sh)
{
    struct ast *ast;

    // ASSIGNMENT_WORD
    if (check_ass(sh->parser))
    {
        ast = ast_prefix_init(NULL);
        struct ast_prefix *ast_prefix = (struct ast_prefix *)ast;
        ast_prefix->ass_word = eat_ass(sh->parser);
        return ast;
    }

    // redir
    ast = parse(sh, RREDIR);

    if (!ast)
        goto not_generated;
    return ast;

not_generated:
    print_not_generated("prefix");
    ast_destroy(ast);
    return NULL;
}

// redir = [IONUMBER] '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>' WORD ;

struct ast *parse_redir(struct sh *sh)
{
    struct ast *ast = ast_redir_init(0, NULL);
    struct ast_redir *ast_redir = (struct ast_redir *)ast;

    // [IONUMBER]
    if (check(sh->parser, IO_NUMBER, false))
    {
        ast_redir->io_number = eat_io_number(sh->parser);
        ast_redir->io_default = false;
    }

    // '>' | '<' | '>>' | '>&' | '<&' | '>|' | '<>'
    if (!check_redir(sh->parser))
        goto not_generated;

    ast_redir->redir_type = eat_redir(sh->parser);

    lexer_move_forward(sh->parser->lexer);

    // WORD
    if (!check(sh->parser, LWORD, false))
    {
        sh->return_code = 2;
        goto error_syntax;
    }

    ast_redir->word = eat_word(sh->parser);

    return ast;

error_syntax:
    sh->return_code = 2;
    perror("Error syntax");
not_generated:
    // no redir was found
    ast_destroy(ast);
    print_not_generated("redir");
    return NULL;
}

// element =
//    WORD
//  | redir
//  ;

struct ast *parse_element(struct sh *sh)
{
    // WORD
    if (check(sh->parser, LWORD, false))
    {
        char *word = eat_word(sh->parser);
        return ast_element_init(word);
    }

    // | redir
    struct ast *redir = parse(sh, RREDIR);
    if (!redir)
        goto not_generated;

    return redir;

not_generated:
    // no element was found
    print_not_generated("element");
    return NULL;
}

// rule_while = 'while' compound_list 'do' compound_list 'done' ;
// rule_until = 'until' compound_list 'do' compound_list 'done' ;

struct ast *parse_while(struct sh *sh)
{
    struct ast *ast = ast_while_init(-1, NULL, NULL);
    struct ast_while *ast_while = (struct ast_while *)ast;

    // 'while'
    if (match(sh->parser, WHILE, true))
        ast_while->type = WHILE;
    else if (match(sh->parser, UNTIL, true)) // 'until'
        ast_while->type = UNTIL;
    else
        goto not_generated;

    // compound_list 'do'
    ast_while->conditions = parse(sh, RCOMPOUND_LIST);
    if (!ast_while->conditions || !match(sh->parser, DO, true))
        goto error_syntax;

    // compound_list
    ast_while->body = parse(sh, RCOMPOUND_LIST);
    if (!ast_while->body)
        goto error_syntax;

    // 'done'
    if (!match(sh->parser, DONE, true))
        goto error_syntax;

    return ast;

error_syntax:
    sh->return_code = 2;
    perror("Error syntax");
not_generated:
    ast_destroy(ast);
    print_not_generated("while");
    return NULL;
}

// rule for = 'for' WORD [';'] | [ {'\n'} 'in' { WORD } ';' | '\n' ] {'\n'}'do'
// compound_list 'done' ;

struct ast *parse_for(struct sh *sh)
{
    struct ast *ast = ast_for_init(NULL, NULL, NULL);
    struct ast_for *ast_for = (struct ast_for *)ast;

    // 'for' WORD
    if (!match(sh->parser, FOR, true))
        goto not_generated;
    if (!check(sh->parser, LWORD, false))
        goto error_syntax;

    ast_for->var = eat_word(sh->parser);

    // [';'] | [ {'\n'} 'in' { WORD } ';' | '\n' ]
    if (!match(sh->parser, SEMICOLON, false))
    {
        // init list of words
        ast_for->words = list_init();

        // {'\n'}
        while (match(sh->parser, NEWLINE, false))
            continue;

        // 'in'
        if (!match(sh->parser, IN, true))
            goto error_syntax;

        // { WORD }
        while (check(sh->parser, LWORD, false))
            list_add(ast_for->words, eat_word(sh->parser));

        // ';' | '\n'
        if (!match(sh->parser, SEMICOLON, false))
        {
            if (!match(sh->parser, NEWLINE, false))
                goto error_syntax;
        }
    }

    // {'\n'}
    while (match(sh->parser, NEWLINE, false))
        continue;

    // 'do'
    if (!match(sh->parser, DO, true))
        goto error_syntax;

    // 'compound_list'
    ast_for->body = parse(sh, RCOMPOUND_LIST);
    if (!ast_for->body) // TODO change ?
        goto error_syntax;

    // 'done'
    if (!match(sh->parser, DONE, true))
        goto error_syntax;

    return ast;

error_syntax:
    sh->return_code = 2;
    perror("Error syntax");
not_generated:
    ast_destroy(ast);
    print_not_generated("for");
    return NULL;
}

// funcdec = WORD '(' ')' {'\n'} shell_command ;

struct ast *parse_funcdec(struct sh *sh)
{
    struct ast *ast = ast_func_init(NULL, NULL);
    struct ast_func *ast_func = (struct ast_func *)ast;

    // WORD '('
    if (!check(sh->parser, LWORD, false) || !check_ll2(sh->parser, LPAR, false))
        goto not_generated;

    ast_func->name = eat_word(sh->parser);

    // '(' ')'
    if (!match(sh->parser, LPAR, false) || !match(sh->parser, RPAR, false))
        goto error_syntax;

    // {'\n'}
    while (match(sh->parser, NEWLINE, false))
        continue;

    // shell_command
    ast_func->body = parse(sh, RSHELL_COMMAND);
    if (!ast_func->body)
        goto error_syntax;

    return ast;

error_syntax:
    sh->return_code = 2;
not_generated:
    // no element was found
    print_not_generated("funcdec");
    ast_destroy(ast);
    return NULL;
}

// rule_case = 'case' WORD {'\n'} 'in' {'\n'} [case_clause] 'esac' ;

struct ast *parse_case(struct sh *sh)
{
    struct ast *ast = ast_case_init(NULL, NULL);
    struct ast_case *ast_case = (struct ast_case *)ast;

    // 'case' WORD
    if (!match(sh->parser, CASE, true) || !check(sh->parser, LWORD, false))
        goto not_generated;

    // gets word
    ast_case->word = eat_word(sh->parser);

    // {'\n'}
    while (match(sh->parser, NEWLINE, false))
        continue;

    // 'in'
    if (!match(sh->parser, IN, false))
        goto error_syntax;

    // {'\n'}
    while (match(sh->parser, NEWLINE, false))
        continue;

    // gets list of items
    ast_case->items = parse_clause(sh);

    // 'esac'
    if (!match(sh->parser, ESAC, true))
        goto error_syntax;

    return ast;

error_syntax:
    sh->return_code = 2;
    perror("Error syntax");
not_generated:
    // no element was found
    print_not_generated("case");
    ast_destroy(ast);
    return NULL;
}

// case_clause = case_item { ';;' {'\n'} case_item } [';;'] {'\n'} ;

struct list *parse_clause(struct sh *sh)
{
    struct list *items = list_init();

    // reserved word
    if (check(sh->parser, ESAC, true))
        return items;

    // case_item
    struct ast *item = parse(sh, RITEM);
    if (!item)
        goto not_generated;
    list_add(items, item);

    // { ';;' {'\n'} case_item } [';;'] {'\n'}
    while (match(sh->parser, DSEMICOLON, false))
    {
        // {'\n'}
        while (match(sh->parser, NEWLINE, false))
            continue;

        // reserved word
        if (check(sh->parser, ESAC, true))
            return items;

        // case_item
        item = parse(sh, RITEM);
        if (!item)
            break;
        list_add(items, item);
    }

    // {'\n'}
    while (match(sh->parser, NEWLINE, false))
        continue;

    return items;

not_generated:
    // no element was found
    print_not_generated("clause");
    list_destroy(items, destroy_ast_void);
    return NULL;
}

// case_item = ['('] WORD { '|' WORD } ')' {'\n'} [compound_list] ;

struct ast *parse_item(struct sh *sh)
{
    struct ast *ast = ast_item_init(list_init(), NULL);
    struct ast_item *ast_item = (struct ast_item *)ast;

    // ['(']
    match(sh->parser, LPAR, false);

    // WORD
    if (!check(sh->parser, LWORD, false))
        goto error_syntax;
    list_add(ast_item->words, eat_word(sh->parser));

    // { '|' WORD }
    while (match(sh->parser, PIPE, false))
    {
        // WORD
        if (!check(sh->parser, LWORD, false))
            goto not_generated;
        list_add(ast_item->words, eat_word(sh->parser));
    }

    // ')'
    if (!match(sh->parser, RPAR, false))
        goto not_generated;

    // {'\n'}
    while (match(sh->parser, NEWLINE, false))
        continue;

    // [compound_list]
    ast_item->body = parse(sh, RCOMPOUND_LIST);

    return ast;

not_generated:
    // no element was found
    print_not_generated("item");
    ast_destroy(ast);
    return NULL;

error_syntax:
    ast_destroy(ast);
    errx(2, "item: error_syntax");
    return NULL;
}
