#include "evaluate_expression.h"
#include <stdio.h>
#include <string.h>

// 11 is 1 sign + 10 digits for an integer value
#define MAX_COMPONENT_LENGTH 11

long separated_components(const char *input_str) {
    long result = 0;

    char pp_str[MAX_LENGTH];
    int len_pp_str;
    get_preprocessed_str(input_str, pp_str, &len_pp_str);

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
    int n_components = 1;
    bool prev_is_digit = true;

    int current_len=1;
    for (int i = 1; pp_str[i] != '\0'; i++) {
        current_len++;
        if (!isdigit(pp_str[i])) {
            n_components++;
            current_len = 1;
        }
        if (current_len > MAX_COMPONENT_LENGTH){
            fprintf(stderr, "Error: Single component exceeds max length of %d.\n", MAX_COMPONENT_LENGTH);
            exit(EXIT_FAILURE);
        }
    }

    if (n_components == 1) {
        printf("single component\n");
        return strtol(pp_str, NULL, 10);
    }

    // MAX_COMPONENT_LENGTH + 1 to take into account the null char
    char components[n_components][MAX_COMPONENT_LENGTH + 1];
    memset(components, 0, n_components*MAX_COMPONENT_LENGTH + 1);

    printf("pp_str: %s\n", pp_str);
    printf("n_components: %d\n", n_components);

    int i = 0, j = 0, k = 0;
    bool new_component = true;
    if (
        isdigit(pp_str[0])
    ){
        components[0][0] = '+';
        k++;
    }
    while (pp_str[i] != '\0') {
        // printf("j: %d", j);
        while ((isdigit(pp_str[i]) || new_component)) {
            // printf("i: %d", i);
            // printf("k: %d\n", k);
            new_component = false;
            components[j][k] = pp_str[i];
            k++;
            i++;
            // printf("components[%d]: %s\n",j, components[j]);
        }
        k = 0;
        new_component = true;
        j++;
    }

    for (int i = 0; i < n_components; i++) {
        printf("%d: %s\n", i, components[i]);
    }
    
    char *pstr;
    for (int i = 0; i < n_components; i++) {
        pstr = components[i];
        pstr++;
        if (components[i][0] == '+'){
            result += strtol(pstr, NULL, 10);
        } else if (components[i][0] == '-'){
            result -= strtol(pstr, NULL, 10);
        }
    }
    
    return result;
}

int main() {
    char expr[] = "2 + 3 + 4 - 3 - 5 -1234567890 + 2";
    printf("%s = %ld", expr, separated_components(expr));
    return 0;
}