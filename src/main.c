// Main file for the Fossil compiler
//
// Ben Champion <bwc3252@rit.edu>

#include <stdio.h>

#include "lexer/lexer.h"
#include "linked_list/list.h"
#include "io/read_file.h"
#include "parser/parser.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "ERROR: no input file\n");
        return 1;
    }
    char *str = read_source_file(argv[1]);
    list_node_t node = tokenize(str);
    list_node_t start = node;
    categorize_tokens(start);
    while (node != NULL) {
        printf("%s %d\n", node->text, node->token_class);
        node = node->next;
    }
    destroy_list(start);
    free(str);
}
