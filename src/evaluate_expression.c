#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#define MAX_LENGTH 256

const char VALID_SYMBOLS[] = "+-";

bool isin(char c, const char *pstr) {
  /*Check if the char c exists in the array pointed by pstr*/
  for (; *pstr != '\0'; pstr++) // pointer arithmetics
  {
    if (c == *pstr) {
      return true;
    }
  }
  return false;
}

void get_preprocessed_str(const char *input_str, char *pp_str,
                          int *len_pp_str) {
  if (*input_str == '\0') {
    fprintf(stderr, "Error: Invalid string. The string is empty");
    exit(EXIT_FAILURE);
  }

  *len_pp_str = 0;

  bool prev_was_symbol = false;
  for (int i = 0; input_str[i] != '\0'; i++) {
    if (input_str[i] == ' ') {
      continue;
    } else if (isdigit(input_str[i]) || isin(input_str[i], VALID_SYMBOLS)) {
      if (isdigit(input_str[i])) {
        prev_was_symbol = false;
      } else if (!prev_was_symbol) {
        prev_was_symbol = true;
      } else {
        fprintf(stderr, "Error: Invalid string. Two consecutive symbols.\n");
        exit(EXIT_FAILURE);
      }

      if (*len_pp_str >= MAX_LENGTH) {
        fprintf(stderr,
                "Error: Invalid string. The input should have at most %d "
                "characters\n",
                MAX_LENGTH);
        exit(EXIT_FAILURE);
      }
      pp_str[*len_pp_str] = input_str[i];
      (*len_pp_str)++;
    } else {
      fprintf(stderr, "Error: Invalid character %c found in string\n",
              input_str[i]);
      exit(EXIT_FAILURE);
    }
  }

  if (!isdigit(pp_str[0])) {
    fprintf(stderr,
            "Error: Invalid string. The first character %c is not a digit.\n",
            pp_str[0]);
    exit(EXIT_FAILURE);
  } else if (!isdigit(pp_str[*len_pp_str - 1])) {
    fprintf(stderr,
            "Error: Invalid string. The last character %c is not a digit.\n",
            pp_str[*len_pp_str]);
    exit(EXIT_FAILURE);
  }
}

long evaluate_expression(const char *input_str) {
  char pp_str[MAX_LENGTH];
  int len_pp_str;
  get_preprocessed_str(input_str, pp_str, &len_pp_str);

  char *end_pp_str = pp_str + len_pp_str;

  char *endptr;
  long left_number = strtol(pp_str, &endptr, 10);
  long right_number;

  char *left_str = endptr;

  int loop = 0;
  // printf("end_pp_str: %p\n", end_pp_str);
  while (*endptr != '\0') {
    // printf("%d:%c:%d,", &left_str, *left_str, (int)*left_str);
    // printf(" %d:%c:%d\n", &endptr, *endptr, (int)*endptr);

    // printf("%p:%c:%d,", left_str, left_str ? *left_str : '?', left_str ?
    // (int)*left_str : -1); printf(" %p:%c:%d\n", endptr, endptr ? *endptr :
    // '?', endptr ? (int)*endptr : -1);

    if (*endptr == '+') {
      right_number = strtol(left_str, &endptr, 10);
      left_number += right_number;
    } else if (*endptr == '-') {
      right_number = strtol(left_str, &endptr, 10);
      left_number -= right_number;
    }

    left_str = endptr + 1;
    loop++;
    if (loop == 10) {
      return 1;
    }
  }

  return left_number;
}