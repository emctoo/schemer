#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define MAX_PROGRAM_LENGTH (1024 * 1024)

typedef struct {
	char code[MAX_PROGRAM_LENGTH];
	int len;
	int iter;
	int row;
	int col;
} program_t;

bool program_load(char *file_name, program_t *program) {
	FILE *f = fopen(file_name, "rb");
	if ( f == NULL) return false;
		
	memset(program->code, 0, MAX_PROGRAM_LENGTH);
	fread(program->code, sizeof(char), MAX_PROGRAM_LENGTH, f);
	fclose(f);
		
	program->iter 	= 0;
	program->row 	= 0;
	program->col 	= 0;
	return true;
}

// ----
#define TOKEN_MAX_LENGTH 16

typedef struct { 
	char token[TOKEN_MAX_LENGTH];
	int len;
} token_t;

bool token_any(program_t *program, token_t *token, char any[]) {
	char c[] = { program->code[program->iter] } ;
	int idx = strspn(c, any);
	if(idx >= strlen(any)) { return false; }
	token->token[0] = c[0];
	token->len = 1;
}

const char reserved[] = "[]{}|";
bool is_reserved(char _c) {
	char c[] = { _c };
	return strspn(c, reserved);
}

const char token_character[] = "()'`,.";
bool is_token_chararcter(char _c) {
	char c[] = { _c };
	return strspn(c, token_character);
}

// comma(,) is also a token_character, with ,@ the same
bool token_comma(program_t *program, token_t *token) {
	char c = program->code[program->iter];
	char n = program->code[program->iter + 1];
	char t = program->code[program->iter + 2];
	if ( c == ',' && isblank(n) ) {
		program->iter++, program->col++;
		token->token[0] = ',', token->len = 1;
		return true;
	} else if ( c == ',' && n == '@'  && isblank(t)) {
		program->iter++, program->col++;
		token->token[0] = ',', token->token[1] = '@', token->len = 2;
		return true;
	}
	return false;
}
	
const char special_initial[] = "!$%&*/:<=>?^_~";
bool is_special_initial(char _c) {
	char c[] = { _c };
	return strspn(c, special_initial);
}

const char special_subquent[] = "+-.@";
bool is_special_subquent(char _c) {
	char c[] = { _c };
	return strspn(c, special_subquent); 
}

// const char peculiar_identifier = "+-..."

/*
bool token_identifier(program_t *program, token_t *token) {
	int token_len = 0;
	while ( isblank(program->code[program->iter] ) {
		token->token[token_len] = program->code[program->iter];
		token->len++;
		if ( token_len = token->len ) { return false; }
		
		program->iter++;
		program->col++;		
	}
	return true;
}

bool token_next(program_t *program, token_t *token) {
	char c = program->code[program->iter], n = program->code[program->iter + 1];
	int iter = program->iter;
	
	if(isblank(c)) { 
		program->iter++;		
		program->col++;
		return true; 
	} else if ( c == '\n') { 
		program->iter++;
		program->row++;
		program->col++;
	} else if ( c == ',' && isblank(n) ) { 
		token->code[0] = c, token->len = 1;
		program->iter++, program->cos++;
		return true;
	} else if ( c == ',' && n == '@' ) { 
		token->code[0] = c, token->code[1] = n, token->len = 2;
		program->iter += 2, program->cos += 2;
		return true;
	} else if ( token_any(program, token, token_prefix) || isalpha(c) ) {
		
	}
	
	
	if(token_any(program, token, token_single)) {
		program->iter++;
		program->col++;
	}  else if ( c == '#' && ( n == '(' || n == 'f' || n == 't')) {		
		token->code[0] = c, token->code[1] = n, token->len = 2;
		program->iter += 2, program->cos += 2;
		return true;
	}
}
*/
int main (void) {
	program_t program;
	program_load("./code.scm", &program);
	printf("%s\n", program.code);
	return 0;
}