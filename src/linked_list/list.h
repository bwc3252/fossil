// Linked list header file

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

struct list_node_s {
    struct list_node_s *prev;
    struct list_node_s *next;
    char *text;
    size_t size;
    size_t capacity;
    int is_token;
    int token_class;
} list_node_s;

typedef struct list_node_s *list_node_t;

list_node_t new_list_node(char *text);

void destroy_list_node(list_node_t node);

void destroy_list(list_node_t node);

#endif
