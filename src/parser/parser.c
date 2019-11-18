#include <stdlib.h>
#include <string.h>

#include "token_classes.h"
#include "../linked_list/list.h"
#include "parser.h"

static int str_in_arr(char *str, char **arr);


static int str_in_arr(char *str, char **arr) {
    // inefficient and bad, but the arrays are small so oh well
    for (int i = 0; arr[i] != NULL; ++ i) {
        if (strcmp(str, arr[i]) == 0) {
            return 1;
        }
    } 
    return 0;
}

void categorize_tokens(list_node_t node) {
    while (node != NULL) {
        if (str_in_arr(node->text, keywords)) {
            node->token_class = KEYWORD;
        }
        else if (str_in_arr(node->text, operators)) {
            node->token_class = OPERATOR;
        }
        else if (str_in_arr(node->text, delimiters)) {
            node->token_class = DELIMITER;
        }
    }
}
