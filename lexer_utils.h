#ifndef LEXER_UTILS_H
#define LEXER_UTILS_H

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void lexer_error(char *str);

bool any_of(char c, char set[]);
// bool while_in(char *code, char set[]);

#endif