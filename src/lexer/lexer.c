// Lexer implementation

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "lexer.h"
#include "../linked_list/list.h"
#include "tokens.h"

static list_node_t split(list_node_t list, char *token);

list_node_t tokenize(char *str) {
    list_node_t node = new_list_node(str);
    for (int i = 0; token_list[i] != NULL; ++ i) {
        char *token = token_list[i];
        list_node_t prev;
        while (node != NULL) {
            prev = node;
            node = split(node, token);
        }
        // go back to start, stripping tabs, spaces etc.
        node = prev;
        list_node_t temp;
        while (node->prev != NULL) {
            temp = node->prev;
            if (strcmp(node->text, NEWLINE) == 0) {
                node->text[0] = ';';
            }
            if (strcmp(node->text, TAB) == 0 || strcmp(node->text, SPACE) == 0 
                    || strcmp(node->text, "") == 0) {
                if (node->next != NULL) {
                    node->next->prev = node->prev;
                }
                if (node->prev != NULL) {
                    node->prev->next = node->next;
                }
                destroy_list_node(node);
            }
            node = temp;
        }
    }
    return node;
}

static list_node_t split(list_node_t node, char *token) {
    size_t len = strlen(token);
    char buf[len + 1];
    char *text = node->text;
    if (len > strlen(text) || node->is_token) {
        return node->next;
    }
    for (int i = 0; i < strlen(text); ++ i) {
        strncpy(buf, text + i, len);
        buf[len] = '\0';
        if (strcmp(buf, token) == 0) {
            // split the node on the token
            list_node_t prev = node->prev;
            list_node_t next = node->next;
            list_node_t after = new_list_node(text + i + len);
            text[i] = '\0';
            list_node_t before = new_list_node(text);
            list_node_t curr = new_list_node(token);
            if (prev != NULL) {
                prev->next = before;
            }
            if (next != NULL) {
                next->prev = after;
            }
            before->prev = prev;
            before->next = curr;
            curr->prev = before;
            curr->next = after;
            curr->is_token = 1;
            after->prev = curr;
            after->next = next;
            destroy_list_node(node);
            return after;
        }
    }
    return node->next;
}
