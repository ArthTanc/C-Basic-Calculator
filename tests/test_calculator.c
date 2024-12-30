#include "../src/evaluate_expression.h"
#include "minunit.h"
#include "tinyexpr.h"

bool compare_evaluation(char *pstr) {
    return evaluate_expression(pstr) == (long)te_interp(pstr, NULL);
}

MU_TEST(test_addition_subtraction) {
    mu_assert(compare_evaluation("2"), "test failed");
    mu_assert(compare_evaluation("2+2+2"), "test failed");
    mu_assert(compare_evaluation("2-2"), "test failed");
    mu_assert(compare_evaluation("3-2"), "test failed");
    mu_assert(compare_evaluation("2-3"), "test failed");
    mu_assert(compare_evaluation("-2+2"), "test failed");
    mu_assert(compare_evaluation("-2"), "test failed");
}

MU_TEST_SUITE(test_suite) { MU_RUN_TEST(test_addition_subtraction); }

int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}