#include "lexer_identifier.h"

identifier_t *alloc_identifier() {	
    identifier_t *id = (identifier_t *) malloc(sizeof(identifier_t) * 1);

    id->str = (char *) malloc(sizeof(char) * max_id_length);
    id->iter = 0;
    
    return id;
}

void dealloc_identifier(identifier_t *id) {
    free(id->str);
    free(id);
}

// a string terminated with NULL is extracted, in str, with iter point to NULL
int extract_identifier(code_t *code, int iter, identifier_t *id) {
	char curr = code->code[iter];	
	if(isalpha(curr) == false ) return -1;

	do {		
		id->str[id->iter] = curr;	
		id->iter++;
		printf("%c, %d\n", curr, id->iter);
		
		// [error] exceed max_id_length
		if(id->iter == max_id_length - 1) {
			lexer_error("exceed max_id_length");
			return -1;
		}

		// 
		curr = code->code[++iter];
		if(isalnum(curr) == false) { 
			id->str[id->iter] = '\0';
			return id->iter; 
		}		
	} while(true);
}