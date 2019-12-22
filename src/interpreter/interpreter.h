#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../parser/ast.h"
#include "../context/context.h"

// struct to hold the fossil "virtual machine"
typedef struct vm_s {
    stack_frame_t stack; // pointer to current stack frame
    void *reg; // A general-purpose "register" to hold results of evaluations.
               // For example, in the assignment a = 2 * a + 4, we would hold
               // the result of the right hand side in the register until we
               // update the value of "a" in the symbol table, at which point
               // we would clear the register using the reset() function.
} *vm_t;

// the main interpreter function
void interpret(ast_node_t root);

// a function for throwing errors
void error(char *str, char *msg);

// constructor and destructor for the vm
vm_t create_vm(void);

void destroy_vm(vm_t vm);

// function to reset vm's register
void reset();

// instruction functions
void eval_int_value(ast_node_t node);
void eval_bool_value(ast_node_t node);
void eval_var_term(ast_node_t node);
void eval_literal_term(ast_node_t node);
void eval_term_primary(ast_node_t node);
void eval_expr_primary(ast_node_t node);
void eval_unary(ast_node_t node);
void eval_mult(ast_node_t node);
void eval_add(ast_node_t node);
void eval_comp(ast_node_t node);
void eval_eq(ast_node_t node);
void eval_and(ast_node_t node);
void eval_or(ast_node_t node);
void eval_expr(ast_node_t node);
void eval_decl_assign(ast_node_t node);
void eval_var_assign(ast_node_t node);
void eval_empty_stmt(ast_node_t node);
void eval_if_stmt(ast_node_t node);
void eval_elifs(ast_node_t node);
void eval_elif_stmt(ast_node_t node);
void eval_else_stmt(ast_node_t node);
void eval_assign_stmt(ast_node_t node);
void eval_expr_stmt(ast_node_t node);
void eval_cond_stmt(ast_node_t node);
void eval_while_stmt(ast_node_t node);
void eval_print_stmt(ast_node_t node);
void eval_throw_stmt(ast_node_t node);
void eval_stmts(ast_node_t node);
void eval_block(ast_node_t node);
void eval_function(ast_node_t node);
void eval_prog(ast_node_t node);

#endif
