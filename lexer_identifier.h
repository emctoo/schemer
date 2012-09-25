#ifndef LEXER_IDENTIFIER_H
#define LEXER_IDENTIFIER_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

#include "code_file.h"
#include "lexer_utils.h"

/* started with letter, followed by any char */
static const int max_id_length = 32;

typedef struct {
	int iter;
	char *str;		
} identifier_t;

identifier_t *alloc_identifier();
void dealloc_identifier(identifier_t *id);

int extract_identifier(code_t *code, int iter, identifier_t *id);

#endif