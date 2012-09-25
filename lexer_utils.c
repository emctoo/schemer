#include "lexer_utils.h"

void lexer_error(char *str) {
	printf("[lexer error] %s\n", str);
}

/* return true, if character c in the set, or return false */ 
bool any_of(char c, char set[]) {
	char _c[] = { c } ;
	int idx = strspn(_c, set);
	return idx >= strlen(set) ? false : true; 
}

/* */