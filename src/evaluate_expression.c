#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 11 is 1 sign + 10 digits for an integer value
#define MAX_COMPONENT_LENGTH 11
#define MAX_LENGTH 256

const char VALID_SYMBOLS[] = "+-*/";

struct Node {
    char operation;
    double numeric;
    struct Node* next;
};

bool isin(char c, const char* pstr) {
    /*Check if the char c exists in the array pointed by pstr*/
    for (; *pstr != '\0'; pstr++)  // pointer arithmetics
    {
        if (c == *pstr) {
            return true;
        }
    }
    return false;
}

void get_preprocessed_str(const char* input_str, char* pp_str) {
    if (*input_str == '\0') {
        fprintf(stderr, "Error: Invalid string. The string is empty");
        exit(EXIT_FAILURE);
    }

    int len_pp_str = 0;
    bool prev_was_symbol = false;
    bool first_char = true;
    for (int i = 0; input_str[i] != '\0'; i++) {
        if (input_str[i] == ' ') {
            continue;
        } else if (first_char) {
            if (isdigit(input_str[i])) {
                pp_str[len_pp_str] = '+';
                len_pp_str++;
            } else if (!isin(input_str[i], "+-")) {
                fprintf(stderr,
                        "Error: Invalid string. The first non whitespace "
                        "character should "
                        "be a digit, + or -\n");
                exit(EXIT_FAILURE);
            }
            pp_str[len_pp_str] = input_str[i];
            len_pp_str++;
            first_char = false;
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
        fprintf(stderr, "Error: Invalid string. The first character %c is not a digit.\n", pp_str[0]);
        exit(EXIT_FAILURE);
    } else if (!isdigit(pp_str[len_pp_str - 1])) {
        fprintf(stderr, "Error: Invalid string. The last character %c is not a digit.\n", pp_str[len_pp_str]);
        exit(EXIT_FAILURE);
    }
}

double evaluate_expression(const char* input_str) {
    char pp_str[MAX_LENGTH + 1];  // one extra in case I need to add a + in the beginning
    get_preprocessed_str(input_str, pp_str);
    // printf("pp_str: %s\n", pp_str);

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
            fprintf(stderr, "Error: Single component exceeds max length of %d.\n", MAX_COMPONENT_LENGTH);
            exit(EXIT_FAILURE);
        }
    }

    // MAX_COMPONENT_LENGTH + 1 to take into account the null char
    struct Node head;
    head.next = NULL;

    struct Node* pnode = &head;
    int i = 0, k = 0;
    char component_str[MAX_COMPONENT_LENGTH + 1];
    memset(component_str, 0, sizeof(component_str));
    char* pstr;
    bool new_component = true;
    while (pp_str[i] != '\0') {
        while ((isdigit(pp_str[i]) || new_component)) {
            new_component = false;
            component_str[k] = pp_str[i];
            k++;
            i++;
        }
        k = 0;
        new_component = true;

        pstr = component_str;

        if (isin(*pstr, "*/")) {
            pnode->operation = *pstr;
            pstr++;
        } else {
            pnode->operation = '+';
        }
        pnode->numeric = strtol(pstr, NULL, 10);

        // printf("%s; operation: %c; numeric: %f\n", component_str,
        // pnode->operation, pnode->numeric);

        // Dynamically allocate memory for the new node
        pnode->next = malloc(sizeof(struct Node));
        pnode = pnode->next;
        memset(component_str, 0, sizeof(component_str));
    }
    pnode->next = NULL;  // To counter act the last next node creation

    pnode = &head;
    // printf("head: %p; op: %c; numeric: %f\n", pnode, pnode->operation,
    // pnode->numeric);

    // [+2] -> [-3] -> [*4] -> [+5] -> [/2] -> [/3]
    // [+2] -> [-3] -> [*4] -> [+5] -> [/2] -> [/3]
    // pnode = [+2]; pnode->next = [-3] => pnode->next->next = [*4]

    while (pnode->next != NULL) {
        if ((pnode->next->next == NULL) || isin(pnode->next->operation, "*/") ||
            !isin(pnode->next->next->operation, "*/")) {
            // combine first and second
            switch (pnode->next->operation) {
                case '+':
                    pnode->numeric += pnode->next->numeric;
                    break;
                case '*':
                    pnode->numeric *= pnode->next->numeric;
                    break;
                case '/':
                    pnode->numeric /= pnode->next->numeric;
                    break;
            }
            pnode->next = pnode->next->next;
        } else {
            // Combine second and third

            switch (pnode->next->next->operation) {
                case '*':
                    pnode->next->numeric *= pnode->next->next->numeric;
                    break;
                case '/':
                    pnode->next->numeric /= pnode->next->next->numeric;
                    break;
            }

            pnode->next->next = pnode->next->next->next;
        }
    }

    return pnode->numeric;
}

// int main() {
//     printf("%f", evaluate_expression("2-3*4+5/2/3"));
//     return 0;
// }
