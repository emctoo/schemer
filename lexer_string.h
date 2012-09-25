#ifndef LEXER_STRING_H
#define LEXER_STRING_H

typedef struct {
    int length;
    int iter;
    char *contents;
} string_t;

const int max_string_length = 512;

string_t *alloc_string();
void dealloc_string(string_t *s);

int extract_string(code_t *code, int iter, string_t *str);

#endif 