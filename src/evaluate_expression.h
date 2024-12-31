#ifndef EVALUATE_EXPRESSION_H
#define EVALUATE_EXPRESSION_H

#include "evaluate_expression.c"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LENGTH 256

// Global variables and constants
extern const char VALID_SYMBOLS[];

// Function declarations
bool isin(char c, const char *pstr);
void get_preprocessed_str(const char *input_str, char *pp_str);
long evaluate_expression(const char *input_str);

#endif // EVALUATE_EXPRESSION_H
