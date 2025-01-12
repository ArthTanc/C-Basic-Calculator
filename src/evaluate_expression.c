#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 11 is 1 sign + 10 digits for an integer value
#define MAX_COMPONENT_LENGTH 11
#define MAX_LENGTH 256

const char VALID_OPERATIONS[] = "+-*/";

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
    int parenthesis_stack = 0;
    bool prev_was_symbol = false;
    bool first_char = true;
    for (int i = 0; input_str[i] != '\0'; i++) {
        if (input_str[i] == ' ') {
            continue;
        } else if (first_char) {
            if (isdigit(input_str[i])) {
                pp_str[len_pp_str] = '+';
                len_pp_str++;
            } else if (input_str[i] == '(') {
                parenthesis_stack++;
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
        } else if (isdigit(input_str[i]) || isin(input_str[i], VALID_OPERATIONS)) {
            if (isdigit(input_str[i])) {
                prev_was_symbol = false;
            } else if (!prev_was_symbol) {
                prev_was_symbol = true;
            } else {
                fprintf(stderr, "Error: Invalid string. Two consecutive operations.\n");
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
        } else if (isin(input_str[i], "()")) {
            switch (input_str[i]) {
                case '(':
                    parenthesis_stack++;
                    break;
                case ')':
                    parenthesis_stack--;
                    if (parenthesis_stack < 0) {
                        fprintf(stderr, "Error: Invalid string. The input has no valid parenthesis\n");
                        exit(EXIT_FAILURE);
                    }
                    break;
            }
            pp_str[len_pp_str] = input_str[i];
            len_pp_str++;
        } else {
            fprintf(stderr, "Error: Invalid character %c found in string\n", input_str[i]);
            exit(EXIT_FAILURE);
        }
    }
    pp_str[len_pp_str] = '\0';

    if (!(isdigit(pp_str[0]) || pp_str[0] == '-' || pp_str[0] == '+' || pp_str[0] == '(')) {
        fprintf(stderr, "Error: Invalid string. The first character %c is not a digit.\n", pp_str[0]);
        exit(EXIT_FAILURE);
    } else if (!(isdigit(pp_str[len_pp_str - 1]) || (pp_str[len_pp_str - 1] == ')'))) {
        fprintf(stderr, "Error: Invalid string. The last character %c is not a digit.\n", pp_str[len_pp_str - 1]);
        exit(EXIT_FAILURE);
    } else if (parenthesis_stack > 0) {
        fprintf(stderr, "Error: Invalid string. The parenthesis are not closed\n");
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
    struct Node* head = malloc(sizeof(struct Node));
    head->next = NULL;

    struct Node* pnode = head;
    int i = 0, k = 0;
    char component_str[MAX_COMPONENT_LENGTH + 1];
    memset(component_str, 0, sizeof(component_str));
    char* pstr;

    while (pp_str[i] != '\0') {
        if (isin(pp_str[i], VALID_OPERATIONS)) {
            pnode->operation = pp_str[i];
            i++;
        } else if (isdigit(pp_str[i])) {
            while (isdigit(pp_str[i])) {
                component_str[k] = pp_str[i];
                k++;
                i++;
            }
            k = 0;

            pnode->numeric = strtol(component_str, NULL, 10);
            if (pnode->operation == '-') {
                pnode->operation = '+';
                pnode->numeric *= -1;
            }

            // printf("%s; operation: %c; numeric: %f\n", component_str,
            // pnode->operation, pnode->numeric);

            // Dynamically allocate memory for the new node
            pnode->next = malloc(sizeof(struct Node));
            pnode = pnode->next;
            memset(component_str, 0, sizeof(component_str));
        } else if (pp_str[i] == '(') {
            int exp_i = 0;
            char parenthesis_exp[MAX_LENGTH + 1];
            memset(parenthesis_exp, 0, sizeof((parenthesis_exp)));
            int parenthesis_stack = 1;

            while (true) {
                i++;

                switch (pp_str[i]) {
                    case '(':
                        parenthesis_stack++;
                        break;
                    case ')':
                        parenthesis_stack--;
                        break;
                }

                if (parenthesis_stack == 0) {
                    i++;
                    break;
                }

                parenthesis_exp[exp_i] = pp_str[i];
                exp_i++;
            }

            pnode->numeric = evaluate_expression(parenthesis_exp);
            if (pnode->operation == '-') {
                pnode->operation = '+';
                pnode->numeric *= -1;
            }

            // Dynamically allocate memory for the new node
            pnode->next = malloc(sizeof(struct Node));
            pnode = pnode->next;
            memset(component_str, 0, sizeof(component_str));
        }
    }

    pnode->next = NULL;  // To counter act the last next node creation

    // printf("head: %p; op: %c; numeric: %f\n", pnode, pnode->operation,
    // pnode->numeric);

    // [+2] -> [-3] -> [*4] -> [+5] -> [/2] -> [/3]
    // [+2] -> [-3] -> [*4] -> [+5] -> [/2] -> [/3]
    // pnode = [+2]; pnode->next = [-3] => pnode->next->next = [*4]

    while (head->next != NULL) {
        if ((head->next->next == NULL) || isin(head->next->operation, "*/") ||
            !isin(head->next->next->operation, "*/")) {
            // combine first and second
            switch (head->next->operation) {
                case '+':
                    head->numeric += head->next->numeric;
                    break;
                case '*':
                    head->numeric *= head->next->numeric;
                    break;
                case '/':
                    head->numeric /= head->next->numeric;
                    break;
            }
            head->next = head->next->next;
        } else {
            // Combine second and third

            switch (head->next->next->operation) {
                case '*':
                    head->next->numeric *= head->next->next->numeric;
                    break;
                case '/':
                    head->next->numeric /= head->next->next->numeric;
                    break;
            }

            head->next->next = head->next->next->next;
        }
    }

    return head->numeric;
}

// int main() {
//     char pp_str[MAX_LENGTH + 1];
//     // double d = evaluate_expression(" -(3*82/3)*(5-36)");
//     // printf("%f\n", d);
//     printf("%f\n", evaluate_expression("(3*82/3)*(5-36)"));
//     return 0;
// }
