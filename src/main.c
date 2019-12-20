// Main file for the Fossil interpreter
//
// Ben Champion <bwc3252@rit.edu>

#include <stdio.h>

#include "lexer/lexer.h"
#include "linked_list/list.h"
#include "io/read_file.h"
#include "parser/parser.h"
#include "context/context.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Error: no input file\n");
        return 1;
    }
    char *str = read_source_file(argv[1]);
    list_node_t node = tokenize(str);
    list_node_t start = node;
    parse(node);
    destroy_list(start);
    free(str);
}
