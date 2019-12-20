// Code to handle context/scoping for variables.
// This is done using a stack of hashmaps. Each hashmap acts as a symbol table
// for whatever function is currently executing; calling a function causes a
// new table to be pushed to the stack, and returning pops the current table.
// 
// The hashmap code is also used in the recursive descent parser to track
// declared variables and their types, in order to catch errors due to
// undeclared variables or mismatched types.

#include <stdlib.h>
#include <string.h>

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
    // NOTE: calloc initializes the array entries to NULL
    return table;
}

table_entry_t new_table_entry(void) {
    table_entry_t entry = malloc(sizeof(struct table_entry_s));
    entry->key = NULL;
    entry->value = NULL;
    entry->next = NULL;
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
    for (int i = 0; i < table->len; ++ i) {
        destroy_table_entry(table->table[i]);
    }
    free(table->table);
    free(table);
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

// TODO: implement rehashing if the table becomes too full
void table_add(symbol_table_t table, char *key, void *value) {
    int ind = hashfunc_str(key) % table->len;
    if (table->table[ind] == NULL) {
        table_entry_t entry = new_table_entry();
        entry->key = calloc(strlen(key) + 1, sizeof(char));
        strcpy(entry->key, key);
        entry->value = value;
        table->table[ind] = entry;
        return;
    }
    table_entry_t e = table->table[ind];
    while (e->next != NULL) {
        if (strcmp(e->key, key) == 0) {
            e->value = value;
            return;
        }
    }
    table_entry_t entry = new_table_entry();
    e->next = entry;
    entry->key = calloc(strlen(key) + 1, sizeof(char));
    strcpy(entry->key, key);
    entry->value = value;
}

table_entry_t table_get(symbol_table_t table, char *key) {
    int ind = hashfunc_str(key) % table->len;
    table_entry_t entry = table->table[ind];
    while (entry != NULL) {
        if (strcmp(entry->key, key) == 0) {
            break;
        }
    }
    return entry;
}
