#include <stdio.h>
#include <stdlib.h>

#include "lexer/lexer.h"

char *read_source_file(char *fname);

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Error: no input file\n");
        return 1;
    }
    char *str = read_source_file(argv[1]);
    token_t token = tokenize(str);
    token_t head = token; // keep this so we can free the memory
    while (token != NULL) {
        printf("%d\t%s\n", token->line_no, token->tok);
        token = token->next;
    }
    free(str);
    destroy_token_list(head);
    return 0;
}

char *read_source_file(char *fname) {
    size_t file_size;
    FILE *fp = fopen(fname, "rb");
    fseek(fp, 0, SEEK_END);
    file_size = ftell(fp);
    rewind(fp);
    char *ret = calloc(file_size + 1, sizeof(char));
    fread(ret, sizeof(char), file_size, fp);
    fclose(fp);
    ret[file_size] = '\0';
    return ret;
}
