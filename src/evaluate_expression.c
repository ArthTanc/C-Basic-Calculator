#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 11 is 1 sign + 10 digits for an integer value
#define MAX_COMPONENT_LENGTH 11
#define MAX_LENGTH 256

const char VALID_SYMBOLS[] = "+-";

struct Node {
    char val[MAX_COMPONENT_LENGTH + 1];
    struct Node *next;
};

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

void get_preprocessed_str(const char *input_str, char *pp_str) {
    if (*input_str == '\0') {
        fprintf(stderr, "Error: Invalid string. The string is empty");
        exit(EXIT_FAILURE);
    }

    int len_pp_str = 0;
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

            if (len_pp_str >= MAX_LENGTH) {
                fprintf(stderr,
                        "Error: Invalid string. The input should have at most %d "
                        "characters\n",
                        MAX_LENGTH);
                exit(EXIT_FAILURE);
            }
            pp_str[len_pp_str] = input_str[i];
            len_pp_str++;
        } else {
            fprintf(stderr, "Error: Invalid character %c found in string\n", input_str[i]);
            exit(EXIT_FAILURE);
        }
    }
    pp_str[len_pp_str] = '\0';

    if (!(isdigit(pp_str[0]) || pp_str[0] == '-' || pp_str[0] == '+')) {
        fprintf(stderr, "Error: Invalid string. The first character %c is not a digit.\n",
                pp_str[0]);
        exit(EXIT_FAILURE);
    } else if (!isdigit(pp_str[len_pp_str - 1])) {
        fprintf(stderr, "Error: Invalid string. The last character %c is not a digit.\n",
                pp_str[len_pp_str]);
        exit(EXIT_FAILURE);
    }
}

long evaluate_expression(const char *input_str) {
    long result = 0;

    char pp_str[MAX_LENGTH];
    get_preprocessed_str(input_str, pp_str);

    /*
        Separate the expression into a list of strings
        "2+4+5-4*3/6"
        ->
        ["+2", "+4", "+5", "-4", "*3", "\4"]

        1. They should be organized into a linked list
        2. The items should be scanned based on priority
        3. If the item next to the right component is not of priority,
        both components can be merged

        Extra:
          - Parenthesis expressions can be treated as an entire component,
          and before it is merged with another component it should be solved
          - For parenthesis use recursion, call itself for the parenthesis and
          append to the list the result of the parenthesis expression
    */

    // Simple version with only addition and subtraction
    bool prev_is_digit = true;

    int current_len = 1;
    for (int i = 1; pp_str[i] != '\0'; i++) {
        current_len++;
        if (!isdigit(pp_str[i])) {
            current_len = 1;
        }
        if (current_len > MAX_COMPONENT_LENGTH) {
            fprintf(stderr, "Error: Single component exceeds max length of %d.\n",
                    MAX_COMPONENT_LENGTH);
            exit(EXIT_FAILURE);
        }
    }

    // MAX_COMPONENT_LENGTH + 1 to take into account the null char
    struct Node head;
    head.next = NULL;
    memset(head.val, 0, MAX_COMPONENT_LENGTH + 1);

    struct Node *pnode = &head;

    int k = 0;
    bool new_component = true;
    if (isdigit(pp_str[0])) {
        head.val[0] = '+';
        k++;
    }

    int i = 0;
    while (pp_str[i] != '\0') {
        while ((isdigit(pp_str[i]) || new_component)) {
            new_component = false;
            pnode->val[k] = pp_str[i];
            k++;
            i++;
        }
        k = 0;
        new_component = true;

        // Dynamically allocate memory for the new node
        pnode->next = malloc(sizeof(struct Node));
        pnode = pnode->next;
        memset(pnode->val, 0, MAX_COMPONENT_LENGTH + 1);
    }

    pnode = &head;

    char *pstr;
    while (pnode->next != NULL) {
        pstr = pnode->val;
        pstr++;

        if (pnode->val[0] == '+') {
            result += strtol(pstr, NULL, 10);
        } else if (pnode->val[0] == '-') {
            result -= strtol(pstr, NULL, 10);
        }

        pnode = pnode->next;
    }

    return result;
}