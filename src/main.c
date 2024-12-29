#include "evaluate_expression.h"

int main() {
    const char input_str[] = "27382342344+299083874 -234234243438435 + 23242344";

    // Validate the string and remove whitespaces

    long result = evaluate_expression(input_str);

    printf("My result: %ld\n", result);

    // Check if I am right
    char cmd[300];
    snprintf(cmd, 300, "echo \"%s\" | bc", input_str);

    long correct_result = system(cmd);

    printf("The result is correct: %s", correct_result == result ? "false" : "true");

    return 0;
}
