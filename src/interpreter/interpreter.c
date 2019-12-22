// The main file for the AST-walking interpreter

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "interpreter.h"
#include "../parser/ast.h"
#include "../context/context.h"
#include "../parser/token_classes.h"

// global variables for program state
vm_t vm; // the virtual machine

void interpret(ast_node_t root) {
    vm = create_vm();
    eval_prog(root);
}

// FIXME move all error handling to a separate file
//void error(char *str, char *msg) {
//    fprintf(stderr, "Error: %s: %s\n", msg, str);
//    exit(EXIT_FAILURE);
//}

// constructor and destructor for the vm
vm_t create_vm(void) {
    vm_t vm = malloc(sizeof(struct vm_s));
    vm->stack = new_stack_frame(NULL);
    vm->reg = NULL;
    return vm;
}

void destroy_vm(vm_t vm) {
    reset();
    destroy_stack(vm->stack);
    free(vm);
}

// function to reset vm's register by freeing whatever dynamically-allocated
// memory was there
void reset() {
    if (vm->reg != NULL) {
        free(vm->reg);
    }
}

// instruction functions
void eval_int_value(ast_node_t node) {
    reset();
    int val = ((int_value_t)node->node)->value;
    vm->reg = malloc(sizeof(int));
    *(int *)vm->reg = val;
}

void eval_bool_value(ast_node_t node) {
    reset();
    int val = ((bool_value_t)node->node)->value;
    vm->reg = malloc(sizeof(int));
    *(int *)vm->reg = val;
}

void eval_var_term(ast_node_t node) {
    // to evaluate a variable, we simply look up its value in the symbol table
    // and load it into the register
    reset();
    if (node->dt == INT) {
        char *name = ((var_term_t)node->node)->name;
        table_entry_t entry = table_get(vm->stack->symbol_table, name);
        int val = *(int *)entry->value;
        vm->reg = malloc(sizeof(int));
        *(int *)vm->reg = val;
    }
    else if (node->dt == BOOL) {
        char *name = ((var_term_t)node->node)->name;
        table_entry_t entry = table_get(vm->stack->symbol_table, name);
        int val = *(int *)entry->value;
        vm->reg = malloc(sizeof(int));
        *(int *)vm->reg = val;
    }
}

void eval_literal_term(ast_node_t node) {
    literal_term_t term_node = (literal_term_t)node->node;
    if (node->dt == INT) {
        eval_int_value(term_node->value);
    }
    else if (node->dt == BOOL) {
        eval_bool_value(term_node->value);
    }
}

void eval_term_primary(ast_node_t node) {
    // the node's child node is either a literal or a var
    ast_node_t term = ((term_primary_t)node->node)->term;
    if (term->nt == VAR_TERM) {
        eval_var_term(term);
    }
    else if (term->nt == LITERAL_TERM) {
        eval_literal_term(term);
    }
}

void eval_expr_primary(ast_node_t node) {
    eval_expr(((expr_primary_t)node->node)->expr);
}

void eval_unary(ast_node_t node) {
    unary_t unary_node = (unary_t)node->node;
    if (unary_node->primary != NULL) {
        // it's just a primary
        if (((ast_node_t)unary_node->primary)->nt == TERM_PRIMARY) {
            eval_term_primary(unary_node->primary);
        }
        else if (((ast_node_t)unary_node->primary)->nt == EXPR_PRIMARY) {
            eval_expr_primary(unary_node->primary);
        }
        return;
    }
    // it must be a unary operation
    reset();
    eval_unary(unary_node->unary);
    if (unary_node->op == NEGATE) {
        *(int *)vm->reg *= -1;
    }
    else {
        // currently, the only other unary operation is boolean "not", so we
        // don't need to check the operation
        *(int *)vm->reg = (*(int *)vm->reg) ^ 1;
    }
}

void eval_mult(ast_node_t node) {
    mult_t mult_node = (mult_t)node->node;
    eval_unary(mult_node->unary);
    if (mult_node->mult == NULL) {
        // check if there is an operation, and if there isn't, just return
        return;
    }
    int left_op = *(int *)vm->reg;
    eval_mult(mult_node->mult);
    if (mult_node->op == MULTIPLICATION) {
        *(int *)vm->reg = left_op * *(int *)vm->reg;
    }
    else {
        // it must be division -- first check for division by zero
        if (*(int *)vm->reg == 0) {
            error("", "Division by 0");
        }
        *(int *)vm->reg = left_op / *(int *)vm->reg;
    }
}

void eval_add(ast_node_t node) {
    add_t add_node = (add_t)node->node;
    eval_mult(add_node->mult);
    if (add_node->add == NULL) {
        // check if there is an operation, and if there isn't, just return
        return;
    }
    int left_op = *(int *)vm->reg;
    eval_add(add_node->add);
    if (add_node->op == ADDITION) {
        *(int *)vm->reg = left_op + *(int *)vm->reg;
    }
    else {
        // it must be subtraction
        *(int *)vm->reg = left_op - *(int *)vm->reg;
    }
}

void eval_comp(ast_node_t node) {
    comp_t comp_node = (comp_t)node->node;
    eval_add(comp_node->add);
    if (comp_node->comp == NULL) {
        // no operation, so just return
        return;
    }
    int left_op = *(int *)vm->reg;
    eval_comp(comp_node->comp);
    int result = left_op - *(int *)vm->reg;
    if (comp_node->op == GREATER_OP) {
        if (result > 0) {
            *(int *)vm->reg = 1;
        }
        else {
            *(int *)vm->reg = 0;
        }
    }
    else if (comp_node->op == GR_EQUAL) {
        if (result >= 0) {
            *(int *)vm->reg = 1;
        }
        else {
            *(int *)vm->reg = 0;
        }
    
    }
    else if (comp_node->op == LESS_EQUAL) {
        if (result <= 0) {
            *(int *)vm->reg = 1;
        }
        else {
            *(int *)vm->reg = 0;
        }
    }
    else if (comp_node->op == LESS_OP) {
        if (result < 0) {
            *(int *)vm->reg = 1;
        }
        else {
            *(int *)vm->reg = 0;
        }
    }
}

void eval_eq(ast_node_t node) {
    eq_t eq_node = (eq_t)node->node;
    eval_comp(eq_node->comp);
    if (eq_node->eq == NULL) {
        // check if there is an operation, and if there isn't, just return
        return;
    }
    int left_op = *(int *)vm->reg;
    eval_eq(eq_node->eq);
    int result = left_op - *(int *)vm->reg;
    if (eq_node->op == EQUAL) {
        if (result == 0) {
            *(int *)vm->reg = 1;
        }
        else {
            *(int *)vm->reg = 0;
        }
    }
    else {
        if (result == 1) {
            *(int *)vm->reg = 1;
        }
        else {
            *(int *)vm->reg = 0;
        }
    }
}

void eval_and(ast_node_t node) {
    and_t and_node = (and_t)node->node;
    eval_eq(and_node->eq);
    if (and_node->and == NULL) {
        return;
    }
    int left_op = *(int *)vm->reg;
    eval_and(and_node->and);
    if (left_op == 1 && *(int *)vm->reg == 1) {
        *(int *)vm->reg = 1;
    }
    else {
        *(int *)vm->reg = 0;
    }
}

void eval_or(ast_node_t node) {
    or_t or_node = (or_t)node->node;
    eval_and(or_node->and);
    if (or_node->or == NULL) {
        return;
    }
    int left_op = *(int *)vm->reg;
    eval_or(or_node->or);
    if (left_op == 1 || *(int *)vm->reg == 1) {
        *(int *)vm->reg = 1;
    }
    else {
        *(int *)vm->reg = 0;
    }
}

void eval_expr(ast_node_t node) {
    expr_t expr_node = (expr_t)node->node;
    eval_or(expr_node->or);
}

void eval_decl_assign(ast_node_t node) {
    decl_assign_t assign_node = (decl_assign_t)node->node;
    char *name = ((var_term_t)((ast_node_t)assign_node->var)->node)->name;
    eval_expr(assign_node->expr);
    int *ptr = malloc(sizeof(int));
    *ptr = *(int *)vm->reg;
    table_add(vm->stack->symbol_table, name, ptr);
}

void eval_var_assign(ast_node_t node) {
    var_assign_t assign_node = (var_assign_t)node->node;
    char *name = ((var_term_t)((ast_node_t)assign_node->var)->node)->name;
    eval_expr(assign_node->expr);
    int *ptr = malloc(sizeof(int));
    *ptr = *(int *)vm->reg;
    table_add(vm->stack->symbol_table, name, ptr);
}

void eval_empty_stmt(ast_node_t node) {}

void eval_cond_stmt(ast_node_t node) {
    cond_stmt_t cond_node = (cond_stmt_t)node->node;
    eval_if_stmt(cond_node->if_stmt);
    if (cond_node->elifs != NULL) {
        eval_elifs(cond_node->elifs);
    }
    if (cond_node->else_stmt != NULL) {
        eval_else_stmt(cond_node->else_stmt);
    }
}

void eval_if_stmt(ast_node_t node) {
    if_stmt_t if_node = (if_stmt_t)node->node;
    eval_expr(if_node->cond);
    if (*(int *)vm->reg) {
        eval_block(if_node->block);
    }
}

void eval_elifs(ast_node_t node) {
    // the result of evaluating the previous condition (either in the initial
    // "if" or a preceding "elif") should still be in the register
    elifs_t elifs_node = (elifs_t)node->node;
    if (!*(int *)vm->reg) {
        eval_elif_stmt(elifs_node->elif_stmt);
    }
    if (elifs_node->elifs != NULL) {
        eval_elifs(elifs_node->elifs);
    }
}

void eval_elif_stmt(ast_node_t node) {
    elif_stmt_t elif_node = (elif_stmt_t)node->node;
    eval_expr(elif_node->cond);
    if (*(int *)vm->reg) {
        eval_block(elif_node->block);
    }
}

void eval_else_stmt(ast_node_t node) {
    if (*(int *)vm->reg == 0) {
        // we should check that the "if" and none of the "elif"s were executed
        return;
    }
    else_stmt_t else_node = (else_stmt_t)node->node;
    eval_block(else_node->block);
}

void eval_assign_stmt(ast_node_t node) {
    // check if it's a declaration assignment or a variable assignment
    assign_stmt_t assign_node = (assign_stmt_t)node->node;
    if (((ast_node_t)assign_node->assignment)->nt == DECL_ASSIGN) {
        eval_decl_assign(assign_node->assignment);
    }
    else {
        eval_var_assign(assign_node->assignment);
    }
}

void eval_expr_stmt(ast_node_t node) {
    expr_stmt_t expr_stmt_node = (expr_stmt_t)node->node;
    eval_expr(expr_stmt_node->expr);
}

void eval_while_stmt(ast_node_t node) {
    while_stmt_t while_node = (while_stmt_t)node->node;
    for (eval_expr(while_node->cond); *(int *)vm->reg; eval_expr(while_node->cond)) {
        eval_block(while_node->block);
    }
}

void eval_print_stmt(ast_node_t node) {
    print_stmt_t print_node = (print_stmt_t)node->node;
    eval_expr(print_node->expr);
    if (((ast_node_t)print_node->expr)->dt == INT) {
        printf("%d\n", *(int *)vm->reg);
    }
    else if (((ast_node_t)print_node->expr)->dt == BOOL) {
        if (*(int *)vm->reg) {
            printf("true\n");
        }
        else {
            printf("false\n");
        }
    }
}

//void eval_throw_stmt(ast_node_t node);

void eval_stmts(ast_node_t node) {
    stmts_t stmts_node = (stmts_t)node->node;
    int nt = ((ast_node_t)stmts_node->stmt)->nt;
    if (nt == WHILE_STMT) {
        eval_while_stmt(stmts_node->stmt);
    }
    else if (nt == PRINT_STMT) {
        eval_print_stmt(stmts_node->stmt);
    }
    else if (nt == COND_STMT) {
        eval_cond_stmt(stmts_node->stmt);
    }
    else if (nt == ASSIGN_STMT) {
        eval_assign_stmt(stmts_node->stmt);
    }
    else if (nt == EXPR_STMT) {
        eval_expr_stmt(stmts_node->stmt);
    }
    else if (nt == EMPTY_STMT) {
        eval_empty_stmt(stmts_node->stmt);
    }
    if (stmts_node->stmts != NULL) {
        eval_stmts(stmts_node->stmts);
    }
}

void eval_block(ast_node_t node) {
    block_t block_node = (block_t)node->node;
    eval_stmts(block_node->stmts);
}

void eval_function(ast_node_t node) {
    function_t function_node = (function_t)node->node;
    eval_block(function_node->block);
}

void eval_prog(ast_node_t node) {
    prog_t prog_node = (prog_t)node->node;
    eval_function(prog_node->func);
}
