#include "../src/evaluate_expression.h"
#include "minunit.h"
#include "tinyexpr.h"

bool compare_evaluation(char* pstr) {
    bool test_result = evaluate_expression(pstr) == te_interp(pstr, NULL);
    if (!test_result) {
        printf("\nTest failed for expression: %s\n", pstr);
        printf("Got result: %f. Expected result: %f\n", evaluate_expression(pstr), te_interp(pstr, NULL));
    }
    return test_result;
}

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
    mu_assert(compare_evaluation("-3+1"), "test failed");
}

MU_TEST(test_multiplication_division) {
    mu_assert(compare_evaluation("2*2"), "test failed");
    mu_assert(compare_evaluation("2*0"), "test failed");
    mu_assert(compare_evaluation("2/2"), "test failed");
    mu_assert(compare_evaluation("1/2"), "test failed");
    mu_assert(compare_evaluation("1/2*3/2/2*3*3*4"), "test failed");
    mu_assert(compare_evaluation("3+92-3*82/3*5-36"), "failed test");
    mu_assert(compare_evaluation("- 2*3/2 + 1*3/4 - 5"), "test failed");
}

MU_TEST(test_power) {
    mu_assert(compare_evaluation("1 ^ 1"), "test failed");
    mu_assert(compare_evaluation("1 ^ 0"), "test failed");
    mu_assert(compare_evaluation("0 ^ 0"), "test failed");
    mu_assert(compare_evaluation("9 ^ 3"), "test failed");
    mu_assert(compare_evaluation("- 3 ^ 4"), "failed test");
    mu_assert(compare_evaluation("3 ^ (-4)"), "test failed");
}

MU_TEST(test_parenthesis) {
    mu_assert(compare_evaluation("-(-2)"), "failed test");
    mu_assert(compare_evaluation("-(2)"), "failed test");
    mu_assert(compare_evaluation("(-2)"), "failed test");
    mu_assert(compare_evaluation("3+(92)"), "failed test");
    mu_assert(compare_evaluation("(92)"), "failed test");
    mu_assert(compare_evaluation("((3))"), "failed test");
    mu_assert(compare_evaluation("- 3 ^ (4)"), "failed test");
    mu_assert(compare_evaluation("- 3 ^ (-4)"), "failed test");
    mu_assert(compare_evaluation("(-3) ^ (-4)"), "failed test");
}

MU_TEST(test_bigger_inputs) {
    mu_assert(compare_evaluation("234324 -234324 + 2342342 / 12341234 * 123424 - 98989 / 1272736"), "test failed");
    mu_assert(compare_evaluation("(234324-234324) + (2342342/12341234) * (123424 - 98989) / 1272736"), "test failed");
    mu_assert(compare_evaluation("(3 + (9 * (8 * (12 + (123 + (2736 * (2342 - (8787 + (9999)))))))))"), "test failed");
    mu_assert(compare_evaluation("3+(92)-(3*82/3)*(5-36)"), "failed test");
    mu_assert(compare_evaluation("-(3*82/3)*(5-36)"), "failed test");
    mu_assert(compare_evaluation("(3*82/3)*(5-36)"), "failed test");
    mu_assert(compare_evaluation("90 + 5 ^ 2 - 3 ^ 4"), "failed test");
}

MU_TEST(test_division_by_zero) {
    mu_assert(compare_evaluation("1/0"), "test failed");
    mu_assert(compare_evaluation("1/(0)"), "test failed");
    mu_assert(compare_evaluation("1/(1 - 1)"), "test failed");
}

MU_TEST_SUITE(test_suite) {
    printf("\nTesting test_addition_subtraction ");
    MU_RUN_TEST(test_addition_subtraction);
    printf("\nTesting test_multiplication_division ");
    MU_RUN_TEST(test_multiplication_division);
    printf("\nTesting test_parenthesis ");
    MU_RUN_TEST(test_parenthesis);
    printf("\nTesting test_power ");
    MU_RUN_TEST(test_power);
    printf("\nTesting test_division_by_zero ");
    MU_RUN_TEST(test_division_by_zero);
    printf("\nTesting test_bigger_inputs ");
    MU_RUN_TEST(test_bigger_inputs);
}

int main() {
    MU_RUN_SUITE(test_suite);
    MU_REPORT();
    return 0;
}