#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROGRAM_LENGTH (1024 * 1024)

typedef struct {
	char code[MAX_PROGRAM_LENGTH];
	int len;
	int iter;
	int row;
	int col;
} program_t;

bool program_load(char *file_name, program_t *program);

// ----
#define TOKEN_MAX_LENGTH 32

typedef enum _token_type { 
	tk_identifier, 
	tk_boolean, 
	tk_number, 
	tk_character, 
	tk_string, 
	tk_comment,
	tk_left_parenthesis, 
	tk_right_parenthesis, 
	tk_sharp_left_parenthesis, 
	tk_apostrophe, 
	tk_grave_accent, 
	tk_comma, 
	tk_comma_at, 
	tk_period, 
	tk_boolean_true, 
	tk_boolean_false
} token_type;

typedef double number_t;

typedef union _data_t {
	char tk_char_single;
	char tk_char_double[2];
	char tk_char_triple[3];
	char *tk_string;
	number_t tk_number;
} data_t;

typedef struct { 
	char token[TOKEN_MAX_LENGTH];
	int len;
	token_type tp;
	data_t value;
} token_t;

void token_output(token_t *token);
bool is_delimiter(char _c);
typedef bool (*filter_func)(char);
int token_while(program_t *program, filter_func func);

// character program->code[program->iter] is in string 'any' or not
bool token_any(program_t *program, token_t *token, char any[]);