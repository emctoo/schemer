#include "code_file.h"

const int max_code_length = 256;

code_t *load_code(char *code_file_name) {
    code_t *code = (code_t *) malloc(sizeof(code_t) * 1);
    code->code = (char *) malloc(sizeof(char) * max_code_length);

    FILE *code_file = fopen(code_file_name, "rb");
    code->length = fread(code->code, sizeof(char), max_code_length, code_file);
    return code;
}

void free_code(code_t *code) {
    free(code->code);
    free(code);
}
