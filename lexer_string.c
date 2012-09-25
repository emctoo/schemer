#include "lexer_string.h"

string_t *alloc_string() {
    string_t *str = (string_t *) malloc(sizeof(string_t) * 1);
    str->contents = (char *) malloc(sizeof(char) * max_string_length);
    str->length = max_string_length;
    str->iter = 0;
    return str;
}

void dealloc_string(string_t *s) {
    free(s->contents);
    free(s);
}

/*FIXME: refactor this function, it's ugly. */ 
int extract_string(code_t *code, int iter, string_t *str) {
    if( code->code[iter] != '"') { return -1; }
    else {
        status = in_string;
        iter++;
    }

    char curr = code->code[iter];
    while(curr != '"') {
        if(curr == '\\') {
            char next = code->code[iter+1];
            if(next == 'n' || next == '\n') {
                str->contents[str->iter] = '\n';
            } else if(next == 't') {
                str->contents[str->iter] = '\t';
            } else {
                str->contents[str->iter] = next;
            }
            iter++;
        } else {
            str->contents[str->iter] = curr;
        }
        str->iter++;
        str->length--;

        iter++;
        curr = code->code[iter];
    }
}

int main(void) {
    code_t *code = load_code("./code.scm");
    printf("%s", code->code);

    string_t *str = alloc_string();
    extract_string(code, 0, str);
    printf("%s", str->contents);
    dealloc_string(str);

    free_code(code);
    return 0;
}
