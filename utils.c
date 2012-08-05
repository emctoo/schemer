#include "utils.h"

void string_output(char *s, int len) {
	for(int i=0; i<len; i++) { putchar(s[i]); }
	putchar('\n');
}