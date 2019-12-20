// Recursive descent parser for fossil
//
// NOTE: Currently, a fossil source file can only consist of a single main
// function. A significant amount of work will need to be done to implement
// functions more generally -- this will require careful handling of variable
// scope, as well as type checking for both return types and parameter types.

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../linked_list/list.h"
#include "parser.h"
#include "ast.h"
#include "token_classes.h"
#include "../context/context.h"

#define DEBUG_PRINT 1

// global variables for keeping track of current token, function parameters
// and return types, local variables, etc.
list_node_t curr;
symbol_table_t symbol_table;

void advance(void) {
    if (curr == NULL) {
        error("", "Reached end of file");
    }
    curr = curr->next;
}

void error(char *str, char *msg) {
    fprintf(stderr, "Error: %s: %s\n", msg, str);
    exit(EXIT_FAILURE);
}

void expect(char *str) {
    if (curr == NULL) {
        error("", "Reached end of file");
    }
    if (strcmp(curr->text, str) != 0) {
        error(curr->text, "Unexpected token");
    }
    advance();
}

ast_node_t ast_node(void) {
    if (DEBUG_PRINT) {
        printf("%s\n", curr->text);
    }
    ast_node_t ret = malloc(sizeof(struct ast_node_s));
    return ret;
}

ast_node_t parse(list_node_t node) {
    curr = node;
    symbol_table = new_symbol_table();
    return prog();
}

ast_node_t prog(void) {
    if (curr == NULL) {
        return NULL;
    }    
    ast_node_t ret = ast_node();
    prog_t prog_node = malloc(sizeof(struct prog_s));
    prog_node->func = function();
    prog_node->prog = prog();
    ret->node = prog_node;
    ret->nt = PROG;
    return ret;
}

ast_node_t function(void) {
    expect(KEYWORD_FN);
    ast_node_t ret = ast_node();
    function_t func_node = malloc(sizeof(struct function_s));
    if (curr->is_keyword) {
        error(curr->text, "Unexpected keyword");
    }
    else if (curr->is_token) {
        error(curr->text, "Unexpected token");
    }
    func_node->name = calloc(strlen(curr->text) + 1, sizeof(char));
    strcpy(func_node->name, curr->text);
    advance();
    expect(DELIM_LEFT_PAR);
    // TODO parameters
    expect(DELIM_RIGHT_PAR);
    func_node->block = block();
    ret->node = func_node;
    ret->nt = FUNCTION;
    ret->dt = NONE;
    return ret;
}

ast_node_t block(void) {
    expect(DELIM_LEFT_CURL);
    ast_node_t ret = ast_node();
    block_t block_node = malloc(sizeof(struct block_s));
    block_node->stmts = stmts();
    expect(DELIM_RIGHT_CURL);
    ret->nt = BLOCK;
    ret->node = block_node;
    return ret;
}

ast_node_t stmts(void) {
    ast_node_t ret = ast_node();
    stmts_t stmts_node = malloc(sizeof(struct stmts_s));
    // determine which *_stmt() function to call
    if (strcmp(curr->text, KEYWORD_WHILE) == 0) {
        stmts_node->stmt = while_stmt();
    }
    else if (strcmp(curr->text, KEYWORD_PRINT) == 0) {
        stmts_node->stmt = print_stmt();
    }
    else if (strcmp(curr->text, "}") == 0) {
        stmts_node->stmt = empty_stmt();
        stmts_node->stmts = NULL;
        ret->node = stmts_node;
        ret->nt = STMTS;
        return ret;
    }    
    else if (strcmp(curr->text, ";") == 0) {
        stmts_node->stmt = empty_stmt();
        advance();
    }
    else if (strcmp(curr->text, KEYWORD_IF) == 0) {
        stmts_node->stmt = cond_stmt();
    }
    else {
        // must be either an expression statement or an assign statement
        // if there is no "=" between the current token and the next semicolon,
        // it must be an expression statement
        list_node_t node = curr;
        int is_assign = 0;
        while (node != NULL && (strcmp(node->text, ";") != 0)) {
            if (strcmp(node->text, KEYWORD_EQ) == 0) {
                is_assign = 1;
            }
            node = node->next;
        }
        if (is_assign) {
            stmts_node->stmt = assign_stmt();
        }
        else {
            stmts_node->stmt = expr_stmt();
        }
    }
    stmts_node->stmts = stmts();
    ret->node = stmts_node;
    ret->nt = STMTS;
    return ret;
}

ast_node_t print_stmt(void) {
    expect(KEYWORD_PRINT);
    ast_node_t ret = ast_node();
    print_stmt_t print_node = malloc(sizeof(struct print_stmt_s));
    print_node->expr = expr();
    ret->node = print_node;
    ret->nt = PRINT_STMT;
    expect(";");
    return ret;
}

ast_node_t while_stmt(void) {
    expect(KEYWORD_WHILE);
    expect(DELIM_LEFT_PAR);
    ast_node_t ret = ast_node();
    while_stmt_t while_node = malloc(sizeof(struct while_stmt_s));
    while_node->cond = expr();
    if (((ast_node_t)while_node->cond)->dt != BOOL) {
        error("", "Invalid type for while loop condition");
    }
    expect(DELIM_RIGHT_PAR);
    while_node->block = block();
    ret->node = while_node;
    ret->nt = WHILE_STMT;
    return ret;
}

ast_node_t cond_stmt(void) {
    ast_node_t ret = ast_node();
    cond_stmt_t cond_node = malloc(sizeof(struct cond_stmt_s));
    cond_node->if_stmt = if_stmt();
    if (strcmp(curr->text, KEYWORD_ELIF) == 0) {
        cond_node->elifs = elifs();
    }
    else {
        cond_node->elifs = NULL;
    }
    if (strcmp(curr->text, KEYWORD_ELSE) == 0) {
        cond_node->else_stmt = else_stmt();
    }
    else {
        cond_node->else_stmt = NULL;
    }
    ret->node = cond_node;
    ret->nt = COND_STMT;
    return ret;
}

ast_node_t if_stmt(void) {
    expect(KEYWORD_IF);
    expect(DELIM_LEFT_PAR);
    ast_node_t ret = ast_node();
    if_stmt_t if_node = malloc(sizeof(struct if_stmt_s));
    if_node->cond = expr();
    if (((ast_node_t)if_node->cond)->dt != BOOL) {
        error("", "Invalid type for if statment condition");
    }
    expect(DELIM_RIGHT_PAR);
    if_node->block = block();
    ret->node = if_node;
    ret->nt = IF_STMT;
    return ret;
}

ast_node_t elifs(void) {
    ast_node_t ret = ast_node();
    elifs_t elifs_node = malloc(sizeof(struct elifs_s));
    elifs_node->elif_stmt = elif_stmt();
    elifs_node->elifs = elifs();
    ret->node = elifs_node;
    ret->nt = ELIFS;
    return ret;
}

ast_node_t elif_stmt(void) {
    expect(KEYWORD_ELIF);
    expect(DELIM_LEFT_PAR);
    ast_node_t ret = ast_node();
    elif_stmt_t elif_node = malloc(sizeof(struct elif_stmt_s));
    elif_node->cond = expr();
    if (((ast_node_t)elif_node->cond)->dt != BOOL) {
        error("", "Invalid type for elif statement condition");
    }
    expect(DELIM_RIGHT_PAR);
    elif_node->block = block();
    ret->node = elif_node;
    ret->nt = ELIF_STMT;
    return ret;
}

ast_node_t else_stmt(void) {
    expect(KEYWORD_ELSE);
    ast_node_t ret = ast_node();
    else_stmt_t else_node = malloc(sizeof(struct else_stmt_s));
    else_node->block = block();
    ret->nt = ELSE_STMT;
    return ret;
}

ast_node_t expr_stmt(void) {
    ast_node_t ret = ast_node();
    expr_stmt_t expr_stmt_node = malloc(sizeof(struct expr_stmt_s));
    expr_stmt_node->expr = expr();
    ret->node = expr_stmt_node;
    ret->nt = EXPR_STMT;
    expect(";");
    return ret;
}

ast_node_t assign_stmt(void) {
    ast_node_t ret = ast_node();
    assign_stmt_t assign_node = malloc(sizeof(struct assign_stmt_s));
    if ((strcmp(curr->text, KEYWORD_BOOL) == 0)
            || (strcmp(curr->text, KEYWORD_INT) == 0)) {
        assign_node->assignment = decl_assign();
    }
    else {
        assign_node->assignment = var_assign();
    }
    ret->node = assign_node;
    ret->nt = ASSIGN_STMT;
    expect(";");
    return ret;
}

ast_node_t empty_stmt(void) {
    ast_node_t ret = ast_node();
    ret->node = NULL;
    ret->nt = EMPTY_STMT;
    return ret;
}

ast_node_t var_assign(void) {
    ast_node_t ret = ast_node();
    var_assign_t assign_node = malloc(sizeof(struct var_assign_s));
    assign_node->var = term();
    if (((ast_node_t)assign_node->var)->nt != VAR_TERM) {
        error("", "Invalid assignment");
    }
    assign_node->assign = EQUALS;
    expect("=");
    assign_node->expr = expr();
    var_term_t var_node = ((ast_node_t)assign_node->var)->node;
    char *name = var_node->name;
    table_entry_t entry = table_get(symbol_table, name);
    if (entry == NULL) {
        error(name, "Assignment to undeclared variable");
    }
    int var_type = *(int *)entry->value;
    if (var_type != ((ast_node_t)assign_node->expr)->dt) {
        error("", "Type error in assignment");
    }
    ret->dt = var_type;
    ret->node = assign_node;
    ret->nt = VAR_ASSIGN;
    return ret;
}

ast_node_t decl_assign(void) {
    ast_node_t ret = ast_node();
    decl_assign_t assign_node = malloc(sizeof(struct decl_assign_s));
    if (strcmp(curr->text, KEYWORD_INT) == 0) {
        ret->dt = INT;
    }
    else if (strcmp(curr->text, KEYWORD_BOOL) == 0) {
        ret->dt = BOOL;
    }
    advance();
    assign_node->var = term();
    char *name = ((var_term_t)((ast_node_t)assign_node->var)->node)->name;
    if (table_get(symbol_table, name) != NULL) {
        error(name, "Variable already declared");
    }
    assign_node->assign = EQUALS;
    expect("=");
    assign_node->expr = expr();
    ((ast_node_t)assign_node->var)->dt = ret->dt;
    int *dt = malloc(sizeof(int));
    *dt = ret->dt;
    table_add(symbol_table, name, dt);
    if (ret->dt != ((ast_node_t)assign_node->expr)->dt) {
        error("", "Type error in assignment");
    }
    ret->node = assign_node;
    ret->nt = DECL_ASSIGN;
    return ret;
}

ast_node_t expr(void) {
    ast_node_t ret = ast_node();
    expr_t expr_node = malloc(sizeof(struct expr_s));
    expr_node->or = or();
    ret->node = expr_node;
    ret->nt = EXPR;
    ret->dt = ((ast_node_t)expr_node->or)->dt;
    return ret;
}

ast_node_t or(void) {
    ast_node_t ret = ast_node();
    or_t or_node = malloc(sizeof(struct or_s));
    or_node->and = and();
    if (strcmp(curr->text, OP_OR) == 0) {
        or_node->or = or();
        if ((((ast_node_t)or_node->and)->dt != BOOL)
                || (((ast_node_t)or_node->or)->dt != BOOL)) {
            error("", "Invalid type for 'or'");
        }
    }
    else {
        or_node->or = NULL;
        ret->dt = ((ast_node_t)or_node->and)->dt;
    }
    ret->node = or_node;
    ret->nt = OR;
    return ret;
}

ast_node_t and(void) {
    ast_node_t ret = ast_node();
    and_t and_node = malloc(sizeof(struct and_s));
    and_node->eq = eq();
    if (strcmp(curr->text, OP_AND) == 0) {
        and_node->and = and();
        if ((((ast_node_t)and_node->eq)->dt != BOOL)
                || (((ast_node_t)and_node->and)->dt != BOOL)) {
            error("", "Invalid type for 'and'");
        }
    }
    else {
        and_node->and = NULL;
        ret->dt = ((ast_node_t)and_node->eq)->dt;
    }
    ret->node = and_node;
    ret->nt = AND;
    return ret;
}

ast_node_t eq(void) {
    ast_node_t ret = ast_node();
    eq_t eq_node = malloc(sizeof(struct eq_s));
    eq_node->comp = comp();
    if (strcmp(curr->text, OP_NOT_EQUAL) == 0) {
        eq_node->op = NOT_EQUAL;
        advance();
        eq_node->eq = eq();
        ret->dt = BOOL;
    }
    else if (strcmp(curr->text, OP_EQUAL) == 0) {
        eq_node->op = EQUAL;
        advance();
        eq_node->eq = eq();
        ret->dt = BOOL;
    }
    else {
        eq_node->op = NO_OP;
        eq_node->eq = NULL;
        ret->dt = ((ast_node_t)eq_node->comp)->dt;
    }
    ret->node = eq_node;
    ret->nt = EQ;
    return ret;
}

ast_node_t comp(void) {
    ast_node_t ret = ast_node();
    comp_t comp_node = malloc(sizeof(struct comp_s));
    comp_node->add = add();
    if (strcmp(curr->text, OP_GREATER) == 0) {
        comp_node->op = GREATER_OP;
        advance();
        comp_node->comp = comp();
        ret->dt = BOOL;
    }
    else if (strcmp(curr->text, OP_GR_EQUAL) == 0) {
        comp_node->op = GR_EQUAL;
        advance();
        comp_node->comp = comp();
        ret->dt = BOOL;
    }
    else if (strcmp(curr->text, OP_LESS_EQUAL) == 0) {
        comp_node->op = LESS_EQUAL;
        advance();
        comp_node->comp = comp();
        ret->dt = BOOL;
    }
    else if (strcmp(curr->text, OP_LESS) == 0) {
        comp_node->op = LESS_OP;
        advance();
        comp_node->comp = comp();
        ret->dt = BOOL;
    }
    else {
        comp_node->op = NO_OP;
        comp_node->comp = NULL;
        ret->dt = ((ast_node_t)comp_node->add)->dt;
    }
    ret->node = comp_node;
    ret->nt = COMP;
    return ret;
}

ast_node_t add(void) {
    ast_node_t ret = ast_node();
    add_t add_node = malloc(sizeof(struct add_s));
    add_node->mult = mult();
    if (strcmp(curr->text, OP_ADD) == 0) {
        add_node->op = ADDITION;
        advance();
        add_node->add = add();
        if ((((ast_node_t)add_node->mult)->dt != INT)
                || (((ast_node_t)add_node->add)->dt != INT)) {
            error("", "Invalid types for addition");
        }
        ret->dt = INT;
    }
    else if (strcmp(curr->text, OP_SUBTRACT) == 0) {
        add_node->op = SUBTRACTION;
        advance();
        add_node->add = add();
        if ((((ast_node_t)add_node->mult)->dt != INT)
                || (((ast_node_t)add_node->add)->dt != INT)) {
            error("", "Invalid types for addition");
        }
        ret->dt = INT;
    }
    else {
        add_node->op = NO_OP;
        add_node->add = NULL;
        ret->dt = ((ast_node_t)add_node->mult)->dt;
    }
    ret->node = add_node;
    ret->nt = ADD;
    return ret;
}

ast_node_t mult(void) {
    ast_node_t ret = ast_node();
    mult_t mult_node = malloc(sizeof(struct mult_s));
    mult_node->unary = unary();
    if (strcmp(curr->text, OP_MULTIPLY) == 0) {
        mult_node->op = MULTIPLICATION;
        advance();
        mult_node->mult = mult();
        if ((((ast_node_t)mult_node->unary)->dt != INT)
                || (((ast_node_t)mult_node->mult)->dt != INT)) {
            error("", "Invalid types for multiplication");
        }
        ret->dt = INT;
    }
    else if (strcmp(curr->text, OP_DIVIDE) == 0) {
        mult_node->op = DIVISION;
        advance();
        mult_node->mult = mult();
        if ((((ast_node_t)mult_node->unary)->dt != INT)
                || (((ast_node_t)mult_node->mult)->dt != INT)) {
            error("", "Invalid types for division");
        }
        ret->dt = INT;
    }
    else {
        mult_node->op = NO_OP;
        mult_node->mult = NULL;
        ret->dt = ((ast_node_t)mult_node->unary)->dt;
    }
    ret->node = mult_node;
    ret->nt = MULT;
    return ret;
}

ast_node_t unary(void) {
    ast_node_t ret = ast_node();
    unary_t unary_node = malloc(sizeof(struct unary_s));
    if (strcmp(curr->text, OP_NEGATE) == 0) {
        unary_node->primary = NULL;
        unary_node->op = NEGATE;
        advance();
        unary_node->unary = unary();
        if (((ast_node_t)unary_node->unary)->dt != INT) {
            error("", "Invalid type for negation");
        }
        ret->dt = INT;
    }
    else if (strcmp(curr->text, OP_NOT) == 0) {
        unary_node->primary = NULL;
        unary_node->op = NOT_OP;
        advance();
        unary_node->unary = unary();
        if (((ast_node_t)unary_node->unary)->dt != BOOL) {
            error("", "Invalid type for 'not'");
        }
        ret->dt = BOOL;
    }
    else {
        unary_node->primary = primary();
        unary_node->op = NO_OP;
        unary_node->unary = NULL;
        ret->dt = ((ast_node_t)unary_node->primary)->dt;
    }
    ret->node = unary_node;
    ret->nt = UNARY;
    return ret;
}

ast_node_t primary(void) {
    ast_node_t ret = ast_node();
    if (strcmp(curr->text, DELIM_LEFT_PAR) == 0) {
        advance();
        expr_primary_t primary_node = malloc(sizeof(struct expr_primary_s));
        primary_node->expr = expr();
        expect(DELIM_RIGHT_PAR);
        ret->dt = ((ast_node_t)primary_node->expr)->dt;
        ret->nt = EXPR_PRIMARY;
        ret->node = primary_node;
        return ret;
    }
    term_primary_t primary_node = malloc(sizeof(struct term_primary_s));
    primary_node->term = term();
    ret->dt = ((ast_node_t)primary_node->term)->dt;
    ret->nt = TERM_PRIMARY;
    ret->node = primary_node;
    return ret;
}

ast_node_t term(void) {
    ast_node_t ret = ast_node();
    if ((strcmp(curr->text, KEYWORD_TRUE) == 0)
            || (strcmp(curr->text, KEYWORD_FALSE) == 0)) {
        literal_term_t term_node = malloc(sizeof(struct literal_term_s));
        term_node->value = bool_value();
        ret->dt = BOOL;
        ret->nt = LITERAL_TERM;
        ret->node = term_node;
        return ret;
    }
    char *endptr;
    strtol(curr->text, &endptr, 10);
    if (*endptr == '\0') {
        literal_term_t term_node = malloc(sizeof(struct literal_term_s));
        term_node->value = int_value();
        ret->dt = INT;
        ret->nt = LITERAL_TERM;
        ret->node = term_node;
        return ret;
    }
    var_term_t term_node = malloc(sizeof(struct var_term_s));
    term_node->name = calloc(strlen(curr->text) + 1, sizeof(char));
    strcpy(term_node->name, curr->text);
    ret->node = term_node;
    table_entry_t entry = table_get(symbol_table, term_node->name);
    if (entry != NULL) {
        ret->dt = *(int *)entry->value;
    }
    ret->nt = VAR_TERM;
    advance();
    return ret;
}

ast_node_t bool_value(void) {
    ast_node_t ret = ast_node();
    bool_value_t bool_node = malloc(sizeof(struct bool_value_s));
    if (strcmp(curr->text, KEYWORD_TRUE) == 0) {
        bool_node->value = 1;
    }
    else {
        bool_node->value = 0;
    }
    ret->node = bool_node;
    ret->nt = BOOL_VALUE;
    advance();
    return ret;
}

ast_node_t int_value(void) {
    ast_node_t ret = ast_node();
    int_value_t int_node = malloc(sizeof(struct int_value_s));
    char *endptr;
    int_node-> value = strtol(curr->text, &endptr, 10);
    ret->node = int_node;
    ret->dt = INT;
    ret->nt = INT_VALUE;
    advance();
    return ret;
}
