// Header file for hashmap and stack code (see context.c)

#ifndef CONTEXT_H
#define CONTEXT_H

#define SYMBOL_TABLE_INIT 10

#include <stdlib.h>

typedef struct table_entry_s {
    char *key;
    void *value;
    struct table_entry_s *next;
} *table_entry_t;

typedef struct symbol_table_s {
    size_t size;
    size_t len;
    table_entry_t *table;
} *symbol_table_t;

typedef struct stack_frame_s {
    symbol_table_t symbol_table;
    struct stack_frame_s *prev;
} *stack_frame_t;

// constructor and destructor function declarations
stack_frame_t new_stack_frame(stack_frame_t top);

symbol_table_t new_symbol_table(void);

table_entry_t new_table_entry(void);

void destroy_stack(stack_frame_t top);

void destroy_stack_frame(stack_frame_t frame);

void destroy_symbol_table(symbol_table_t table);

void destroy_table_entry(table_entry_t entry);

// a useful hash function for strings
unsigned long hashfunc_str(char *str);

// functions for interacting with the symbol table

void table_add(symbol_table_t table, char *key, void *value);

table_entry_t table_get(symbol_table_t table, char *key);

#endif
