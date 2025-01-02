#include "../src/evaluate_expression.h"
#include "minunit.h"
#include "tinyexpr.h"

bool compare_evaluation(char* pstr) { return evaluate_expression(pstr) == te_interp(pstr, NULL); }

MU_TEST(test_addition_subtraction) {
    mu_assert(compare_evaluation("2"), "test failed");
    mu_assert(compare_evaluation("-2"), "test failed");
    mu_assert(compare_evaluation("+2"), "test failed");
    mu_assert(compare_evaluation("2+2+2"), "test failed");
    mu_assert(compare_evaluation("2-2"), "test failed");
    mu_assert(compare_evaluation("3-2"), "test failed");
    mu_assert(compare_evaluation("2-3"), "test failed");
    mu_assert(compare_evaluation("+2-3+5-4-10"), "test failed");
    mu_assert(compare_evaluation("2-3-1"), "test failed");
}

MU_TEST(test_multiplication_division) {
    mu_assert(compare_evaluation("2*2"), "test failed");
    mu_assert(compare_evaluation("2*0"), "test failed");
    mu_assert(compare_evaluation("2/2"), "test failed");
    mu_assert(compare_evaluation("1/2"), "test failed");
    mu_assert(compare_evaluation("1/2*3/2/2*3*3*4"), "test failed");
    mu_assert(compare_evaluation("-2*3/2 + 1*3/4 -5"), "test failed");
}

MU_TEST(test_bigger_inputs) {
    mu_assert(compare_evaluation("234324 -234324 + 2342342 / 12341234 * 123424 - 98989 / 1272736"), "test failed");
}

MU_TEST_SUITE(test_suite) {
    MU_RUN_TEST(test_addition_subtraction);
    MU_RUN_TEST(test_multiplication_division);
    MU_RUN_TEST(test_bigger_inputs);
}

int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}