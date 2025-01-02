#ifndef EVALUATE_EXPRESSION_H
#define EVALUATE_EXPRESSION_H

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "evaluate_expression.c"

#define MAX_LENGTH 256

// Global variables and constants
extern const char VALID_SYMBOLS[];

// Function declarations
bool isin(char c, const char* pstr);
void get_preprocessed_str(const char* input_str, char* pp_str);
double evaluate_expression(const char* input_str);

#endif  // EVALUATE_EXPRESSION_H
