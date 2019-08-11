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
    char *text;
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
            text = node->text;
            temp = node->prev;
            // replace newlines with semicolons
            if (strcmp(text, NEWLINE) == 0) {
                text[0] = ';';
            }
            // remove redundant semicolons
            if (strcmp(node->text, TAB) == 0 
                    || strcmp(text, SPACE) == 0 
                    || strcmp(text, EMPTY) == 0
                    || (strcmp(text, ";") == 0 && strcmp(node->prev->text, ";") == 0)
                    || (strcmp(text, ";") == 0 && strcmp(node->prev->text, LEFT_PAR) == 0)
                    || (strcmp(text, ";") == 0 && strcmp(node->prev->text, RIGHT_PAR) == 0)
                    || (strcmp(text, ";") == 0 && strcmp(node->prev->text, LEFT_BR) == 0)
                    || (strcmp(text, ";") == 0 && strcmp(node->prev->text, RIGHT_BR) == 0)
                    || (strcmp(text, ";") == 0 && strcmp(node->prev->text, LEFT_CURL) == 0)
                    || (strcmp(text, ";") == 0 && strcmp(node->prev->text, RIGHT_CURL) == 0)) {
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
        // if there was extra stuff at the start of the file, it may not have been
        // caught yet, so we get rid of it here
        while (node->next != NULL &&
                (strcmp(node->text, NEWLINE) == 0 
                || strcmp(node->text, SEMICOLON) == 0
                || strcmp(node->text, EMPTY) == 0
                || strcmp(node->text, SPACE) == 0)) {
                temp = node->next;
                destroy_list_node(node);
                node = temp;
                node->prev = NULL;
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
