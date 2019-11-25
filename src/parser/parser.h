#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "../linked_list/list.h"

// The main parsing function
ast_node_t parse(list_node_t node);

// A function to advance the current position in the token list and check for
// the end of the list
void advance(void);

// A function to consume a node and check that it matches a given token
void expect(char *str);

// A function to print a syntax error message and abort if the tokens cannot
// be matched
void error(char *str, char *msg);

// A function to make a generic AST node
ast_node_t ast_node(void);

// The rest of these functions are for parsing specific rules in the grammar
// and returning a corresponding AST node

ast_node_t int_value(void);

ast_node_t bool_value(void);

ast_node_t term(void);

ast_node_t primary(void);

ast_node_t unary(void);

ast_node_t mult(void);

ast_node_t add(void);

ast_node_t comp(void);

ast_node_t eq(void);

ast_node_t and(void);

ast_node_t or(void);

ast_node_t expr(void); // TODO here

ast_node_t decl_assign(void);

ast_node_t var_assign(void);

ast_node_t empty_stmt(void);

ast_node_t assign_stmt(void);

ast_node_t expr_stmt(void);

ast_node_t if_stmt(void);

ast_node_t elifs(void);

ast_node_t elif_stmt(void);

ast_node_t else_stmt(void);

ast_node_t cond_stmt(void);

ast_node_t while_stmt(void);

ast_node_t print_stmt(void);

//ast_node_t throw_stmt(void);

ast_node_t stmts(void);

ast_node_t block(void);

ast_node_t function(void);

ast_node_t prog(void);

#endif
