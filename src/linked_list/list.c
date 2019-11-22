// Linked list implementation

#include <stdlib.h>
#include <string.h>

#include "list.h"

list_node_t new_list_node(char *text, int line_no) {
    list_node_t node = malloc(sizeof(struct list_node_s));
    node->prev = NULL;
    node->next = NULL;
    node->size = strlen(text);
    node->capacity = node->size;
    node->text = calloc(node->size + 1, sizeof(char));
    strcpy(node->text, text);
    node->text[node->size] = '\0';
    node->is_token = 0;
    node->token_class = -1; // see src/parser/token_classes.h
    node->line_no = line_no;
    return node;
}

void destroy_list_node(list_node_t node) {
    free(node->text);
    free(node);
}

void destroy_list(list_node_t node) {
    list_node_t temp;
    while (node != NULL) {
        temp = node->next;
        destroy_list_node(node);
        node = temp;
    }
}
