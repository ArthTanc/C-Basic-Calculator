#include "evaluate_expression.h"

int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "Error: No expression was given");
        exit(EXIT_FAILURE);
    } else if (argc >= 3) {
        fprintf(stderr, "Error: Too much command line arguments given");
        exit(EXIT_FAILURE);
    }

    long result = evaluate_expression(argv[1]);

    printf("My result: %ld\n", result);

    return 0;
}
