#include "lexer.h"

bool program_load(char *file_name, program_t *program) {
	FILE *f = fopen(file_name, "rb");
	if ( f == NULL) return false;
		
	memset(program->code, 0, MAX_PROGRAM_LENGTH);
	fread(program->code, sizeof(char), MAX_PROGRAM_LENGTH, f);
	fclose(f);
		
	program->iter 	= 0;
	program->row 	= 1;
	program->col 	= 1;
	return true;
}

void token_output(token_t *token) {
	switch(token->tp) {
		case tk_string: {	
			printf("%s, %s", "string", token->value.tk_string);
			break;
		}
		case tk_comment: {
			printf("comment");
			break;
		}
	}
}

	
bool token_loop_until_blank(program_t *program, token_t *token) {
	char *code = program->code;
	int iter = 0;
	while ( isblank(code[iter]) == false ) {
		if ( program->iter + iter == TOKEN_MAX_LENGTH) { return false; }
		token->token[iter] = code[program->iter + iter];
		iter++;
	}
}

bool token_any(program_t *program, token_t *token, char any[]) {
	char c[] = { program->code[program->iter] } ;
	int idx = strspn(c, any);
	if(idx >= strlen(any)) { return false; }
	token->token[0] = c[0];
	token->len = 1;
}

const char delimiter[] = "()\";\n ";
bool is_delimiter(char _c) {
	char c[] = { _c };
	return strspn(c, delimiter);
}

const char reserved[] = "[]{}|";
bool is_reserved(char _c) {
	char c[] = { _c };
	return strspn(c, reserved);
}

/* #(, #t, #f, #\<char>, #\<char name>, #i, #e, #b, #o, #d, #x */
bool token_sharp(program_t *program, token_t *token) {
	char _1 = program->code[program->iter];	
	char _2 = program->code[program->iter + 1];
	char _3 = program->code[program->iter + 2];

	if ( _1 != '#' ) { return false; }
	if ( _2 == '(' && is_delimiter(_3) ) {
		token->tp = tk_sharp_left_parenthesis;
		return true;
	}
	if ( _2 == 't' && is_delimiter(_3) ) {
		token->tp = tk_sharp_left_parenthesis;
		return true;
	}
	if ( _2 == 'f' && is_delimiter(_3) ) {
		token->tp = tk_sharp_left_parenthesis;
		return true;
	}	
}

const char token_single[] = "()#'`,.";
bool is_token_single(char _c) {
	char c[] = { _c };
	return strspn(c, token_single);
}

/* single as token

tk_left_parenthesis			(
tk_right_parenthesis, 		)
tk_sharp_left_parenthesis, 	#( -- not included here
tk_apostrophe, 				'
tk_grave_accent, 			`
tk_comma, 					,  -- not included here
tk_comma_at, 				,( -- not included here
tk_period					.

*/ 

bool token_parenthesis(program_t *program, token_t *token) {
	char _1 = program->code[program->iter];
	char _2 = program->code[program->iter + 1];	
	switch(_1) {
		case '(': { token->tp = tk_left_parenthesis; break; }
		case ')': { token->tp = tk_right_parenthesis; break; }
		case '#': { token_sharp(program, token); break; }
	}
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

bool token_period(program_t *program, token_t *token) {
	char c = program->code[program->iter];
	char n = program->code[program->iter + 1];
	char *code = program->code;
	if( c == '.' && isblank(n) ) {
		program->iter++, program->col++;		
		token->token[0] = '.', token->len = 1;
		return true;
	} else if ( strncmp(code, "...", 3) == 0 ) {
		program->iter += 3, program->col += 3;
		strncpy(token->token, ".", 3);
		token->len = 3;	
		return true;
	}
	return false;
}


/* ---- character #\<character name>

#\nul 			Unicode character 0
#\alarm 		Unicode character 7
#\backspace 	Unicode character 8
#\tab 			Unicode character 9
#\newline 		Unicode character 10 (newline character)
#\linefeed 		Unicode character 10
#\vtab 			Unicode character 11
#\page 			Unicode character 12
#\return 		Unicode character 13
#\esc 			Unicode character 27
#\space 		Unicode character 32 (space character)
#\delete 		Unicode character 127

-- not supported
#\xhh 			character encoded in hexadecimal (>= 1 hexadecimal digit)
#\uhhhh 		character encoded in hexadecimal (exactly 4 hexadecimal digits)
#\Uhhhhhhhh 	character encoded in hexadecimal (exactly 8 hexadecimal digits)

*/

bool token_character(program_t *program, token_t *token) {
	char _1 = program->code[program->iter];
	char _2 = program->code[program->iter + 1];
	char _3 = program->code[program->iter + 2];
	char _4 = program->code[program->iter + 3];
	if ( _1 == '#' && _2 == '\\' ) {
		if( _3 > 32 && _3 < 127 && isblank(_4) ) {
			program->iter += 2, program->col += 2;
			token->token[0] = _3, token->len = 1, token->tp = tk_character;
			return true;
		}		
		token_loop_until_blank(program, token);
		token->len = 1, token->tp = tk_character;
		return true;
	}
	return false;
}
		

/* ----  string escape code

\a 			Unicode character 7
\b 			Unicode character 8
\t 			Unicode character 9
\n 			Unicode character 10 (newline character)
\v 			Unicode character 11
\f 			Unicode character 12
\r 			Unicode character 13
\" 			"
\\ 			\
\| 			|
\? 			?

\ooo 		character encoded in octal (1 to 3 octal digits, first digit must 
			be less than 4 when there are 3 octal digits)

\xhh 		character encoded in hexadecimal (>= 1 hexadecimal digit)
\uhhhh 		character encoded in hexadecimal (exactly 4 hexadecimal digits)
\Uhhhhhhhh 	character encoded in hexadecimal (exactly 8 hexadecimal digits)
\<space> 	Unicode character 32 (space character)

\<newline><whitespace-except-newline>*
			This sequence expands to nothing (it is useful for splitting a long 
			string literal on multiple lines while respecting proper indentation
			of the source code)

*/

/* "<any chars other than \ or ">"
 * only support two kind of escape, \\ and \" 
 * or escape anything behind \
 */ 
bool token_string(program_t *program, token_t *token) {
	if ( program->code[program->iter] != '"' ) return false;
	program->iter++, program->col++;
	
	char *code = program->code + program->iter;
	int iter = program->iter;
	
	int length = 64, counter = 0;
	char *buffer = (char *) malloc(sizeof(char) * length);
	while( true ) {
		if ( iter == length ) { 
			length += 64;
			buffer = (char *) realloc( buffer, length);
		} 
		
		if ( code[iter] == '"' ) { break; } 
		
		if ( code[iter] == '\\' ) { 
			buffer[counter] = code[iter+1]; 
			iter += 2, counter++; 
		} 
		
		buffer[counter] = code[iter];
		counter++, iter++;		
	}
	
	buffer = (char *) realloc( buffer, counter);
	token->value.tk_string = buffer;	
	return true;
}

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

*/

void token_comment(program_t *program) {
	if(program->code[program->iter] != ';') return;
	printf("[comment] (%d, %d)", program->row, program->col);
	while(program->code[program->iter] != '\n') {		
		// printf("%c @(%d, %d)\n", program->code[program->iter], program->row, program->col);
		program->iter++, program->col++;
	}	
	printf("-- (%d, %d)\n", program->row, program->col);
	program->iter++, program->row++, program->col = 1;
}

void lexer_error(char *s, int r, int c) { 
	printf("[%s] @ (%d, %d)\n", s, r, c); 
	exit(0); 
}

bool token_next(program_t *program, token_t *token) {
	char c = program->code[program->iter], n = program->code[program->iter + 1];
	int iter = program->iter;
	
	printf("{%s} %c\n", __FUNCTION__, c);
	if( is_reserved(c)) { lexer_error("reserved character", program->row, program->col); }
	if( c == ';' ) { token_comment(program); return true; }
}

int main (void) {	
	program_t program;
	program_load("./code.scm", &program);
	printf("%s\n", program.code);
	
	bool lexer_status = true;
	while(lexer_status == true) {		
		token_t token;
		lexer_status = token_next(&program, &token);
		if(lexer_status == true) { token_output(&token); }
	}
	return 0;
}