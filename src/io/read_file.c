// Source code file reading

#include <stdio.h>
#include <stdlib.h>

#include "read_file.h"

/* Read contents of fname into a dynamically-allocated buffer and return it */
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
