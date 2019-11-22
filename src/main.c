// Main file for the Fossil compiler
//
// Ben Champion <bwc3252@rit.edu>

#include <stdio.h>

#include "lexer/lexer.h"
#include "linked_list/list.h"
#include "io/read_file.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "ERROR: no input file\n");
        return 1;
    }
    char *str = read_source_file(argv[1]);
    list_node_t node = tokenize(str);
    list_node_t start = node;
    while (node != NULL) {
        printf("%s\n", node->text);
        node = node->next;
    }
    destroy_list(start);
    free(str);
}
