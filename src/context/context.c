// Code to handle context/scoping for variables.
// This is done using a stack of hashmaps. Each hashmap acts as a symbol table
// for whatever function is currently executing; calling a function causes a
// new table to be pushed to the stack, and returning pops the current table.
// 
// The hashmap code is also used in the recursive descent parser to track
// declared variables and their type, in order to catch errors due to
// undeclared variables or mismatched types.

#include <stdlib.h>

#include "context.h"

stack_frame_t new_stack_frame(stack_frame_t top) {
    stack_frame_t frame = malloc(sizeof(struct stack_frame_s));
    frame->symbol_table = new_symbol_table();
    frame->prev = top;
    return frame;
}

symbol_table_t new_symbol_table(void) {
    symbol_table_t table = malloc(sizeof(struct symbol_table_s));
    table->size = 0;
    table->len = SYMBOL_TABLE_INIT;
    table->table = calloc(SYMBOL_TABLE_INIT, sizeof(table_entry_t *));
    return table;
}

table_entry_t new_table_entry(void) {
    table_entry_t entry = malloc(sizeof(struct table_entry_s));
    entry->key = NULL;
    entry->value = NULL;
    return entry;
}

void destroy_stack(stack_frame_t top) {
    stack_frame_t temp;
    while (top != NULL) {
        temp = top->prev;
        destroy_stack_frame(top);
        top = temp;
    }
}

void destroy_stack_frame(stack_frame_t frame) {
    destroy_symbol_table(frame->symbol_table);
    free(frame);
}

void destroy_symbol_table(symbol_table_t table) {
    
}

void destroy_table_entry(table_entry_t entry) {
    if (entry->key != NULL) {
        free(entry->key);
    }
    if (entry->value != NULL) {
        free(entry->value);
    }
    free(entry);
}

// The djb2 hash function
// For most purposes this is not the best choice, but it's:
//   a) fast
//   b) simple
// For this specific use case, it's more than enough
unsigned long hashfunc_str(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}
