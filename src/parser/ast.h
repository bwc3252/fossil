#ifndef AST_H
#define AST_H

#include <stdlib.h>

enum node_type_e {
    INT_VALUE,
    BOOL_VALUE,
    VAR_TERM,
    LITERAL_TERM,
    TERM_PRIMARY,
    EXPR_PRIMARY,
    UNARY,
    MULT,
    ADD,
    COMP,
    EQ,
    AND,
    OR,
    EXPR,
    DECL_ASSIGN,
    VAR_ASSIGN,
    EMPTY_STMT,
    IF_STMT,
    ELIFS,
    ELIF_STMT,
    ASSIGN_STMT,
    EXPR_STMT,
    ELSE_STMT,
    COND_STMT,
    WHILE_STMT,
    PRINT_STMT,
    THROW_STMT,
    STMTS,
    BLOCK,
    FUNCTION,
    PROG
};

enum data_type_e {
    INT,
    BOOL,
    NONE
};

typedef enum node_type_e node_type;
typedef enum data_type_e data_type;

// a wrapper struct to hold all AST nodes
typedef struct ast_node_s {
    node_type nt;
    data_type dt;
    void *node;
} *ast_node_t;

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// AST node structs: each of these structs approximately corresponds to a //
// production in the grammar (notes/grammar.txt).                         //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

typedef struct int_value_s {
    int value;
} *int_value_t;

typedef struct bool_value_s {
    int value;
} *bool_value_t;

typedef struct var_term_s {
    char *name;
} *var_term_t;

typedef struct literal_term_s {
    void *value;
} * literal_term_t;

typedef struct term_primary_s {
    void *term;
} *term_primary_t;

typedef struct expr_primary_s {
    void *expr;
} *expr_primary_t;

typedef struct unary_s {
    void *primary;
    int op;
    void *unary;
} *unary_t;

typedef struct mult_s {
    void *unary;
    int op;
    void *mult;
} *mult_t;

typedef struct add_s {
    void *mult;
    int op;
    void *add;
} *add_t;

typedef struct comp_s {
    void *add;
    int op;
    void *comp;
} *comp_t;

typedef struct eq_s {
    void *comp;
    int op;
    void *eq;
} *eq_t;

typedef struct and_s {
    void *eq;
    void *and;
} *and_t;

typedef struct or_s {
    void *and;
    void *or;
} *or_t;

typedef struct expr_s {
    void *or;
} *expr_t;

typedef struct decl_assign_s {
    void *var;
    int assign;
    void *expr;
} *decl_assign_t;

typedef struct var_assign_s {
    void *var;
    int assign;
    void *expr;
} *var_assign_t;

typedef struct assign_stmt_s {
    void *assignment;
} *assign_stmt_t;

typedef struct expr_stmt_s {
    void *expr;
} *expr_stmt_t;

typedef struct if_stmt_s {
    void *cond;
    void *block;
} *if_stmt_t;

typedef struct elifs_s {
    void *elif_stmt;
    void *elifs;
} *elifs_t;

typedef struct elif_stmt_s {
    void *cond;
    void *block;
} *elif_stmt_t;

typedef struct else_stmt_s {
    void *block;
} *else_stmt_t;

typedef struct cond_stmt_s {
    void *if_stmt;
    void *elifs;
    void *else_stmt;
} *cond_stmt_t;

typedef struct while_stmt_s {
    void *cond;
    void *block;
} *while_stmt_t;

typedef struct print_stmt_s {
    void *expr;
} *print_stmt_t;

//typedef struct throw_stmt_s {
//    void *expr;
//} *throw_stmt_t;

typedef struct stmts_s {
    void *stmt;
    void *stmts;
} *stmts_t;

typedef struct block_s {
    void *stmts;
} *block_t;

typedef struct function_s {
    char *name;
    void *params;
    void *block;
} *function_t;

typedef struct prog_s {
    void *func;
    void *prog;
} *prog_t;

#endif
