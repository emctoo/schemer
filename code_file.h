#ifndef CODE_FILE_H
#define CODE_FILE_H

#include <stdlib.h>
#include <stdio.h>

typedef struct {
    int length;
    char *code;
} code_t;

code_t *load_code(char *code_file_name);
void free_code(code_t *code);

#endif