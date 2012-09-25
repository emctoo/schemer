#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "lexer_utils.h"
#include "code_file.h"
#include "lexer_identifier.h"

static enum {
    in_string, in_number, in_identifier
} status;

// -- 
int main(void) {
	/* 
	bool b = any_of('c', "abc");
	printf("%d\n", b);
	*/ 
	/*
	char *s1 = "adaeda ; a;k;j;";
	char *s2 = "abcde ;";
	int len = strspn(s1, s2);
	printf("%d\n", len);
	*/ 
	// printf("%d\n", isalpha('3'));
    code_t *code = load_code("./code.scm");
    // printf("%s", code->code);

    identifier_t *id = alloc_identifier();
    int iter = 0;
    extract_identifier(code, iter, id);
    dealloc_identifier(id);

    free_code(code);
    return 0;

	return 0;
}