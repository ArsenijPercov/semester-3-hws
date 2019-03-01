/*
hw1/tests/check_rpn-stack.c

Author: Dushan Terzikj
 */

#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "rpn.h"

/* region GLOBALS */

typedef struct {
    char x[255];	/* input x */
    int r;	        /* result r */
} test_case_t;

/* endregion */

/* region TEST */

START_TEST(test_expr_token_eval)
{

    int len, i, rc;

    test_case_t tests[] = {
        {"384", RPN_OK},
        {"1 2 +", RPN_OK},
        {"1 2 + 3 *", RPN_OK},
        {"3 5 * 15 /", RPN_OK},
        {"0 0 - 5 /", RPN_OK},
        {"5 2 - 3 + 2 /", RPN_OK},
        {"3 2 + a", RPN_INVALID_TOKEN},
        {"3 0 /", RPN_ARITHMETIC_ERROR},
        {"1 2 + 3", RPN_MISSING_OPERATOR},
        {"1 2 + *", RPN_MISSING_OPERAND}
    };
    len = sizeof(tests)/sizeof(tests[0]);

    char *res[] = {
        "384",
        "3",
        "9",
        "1",
        "0",
        "3",
        "invalid token",
        "arithmetic error",
        "missing operator",
        "missing operand"
    };

    for (i = 0; i < len; i++) {
        char *result = (char*) malloc(sizeof(char)*255);

        rc = rpn_eval_expr(tests[i].x, &result);
        printf("-----------------------------");
        printf("\nRunning test: %s\nCorrect code: %d\nProduced code: %d\n", 
            tests[i].x, tests[i].r, rc);
        printf("Correct result: %s\nProduced result: %s\n", res[i], result);
        ck_assert_int_eq(rc, tests[i].r);
        printf("Test passed\n");
        ck_assert_str_eq(result, res[i]);
        printf("Result passed\n");
        free(result);
    }
}
END_TEST

/* endregion */

/* region SUITE */

static Suite* gdb_suite(void) {

   Suite* s;
   TCase *tc_expr_token_eval;

   // Create suite and test cases
   s = suite_create("all");
   tc_expr_token_eval = tcase_create("expr_token_eval");
   

   // Add test cases to suite
   tcase_add_test(tc_expr_token_eval, test_expr_token_eval);
   suite_add_tcase(s, tc_expr_token_eval);

   return s;
}

/* endregion */

/* region MAIN */

int main(void){

   int num_failed;
   Suite* s;
   SRunner *sr;

   s = gdb_suite();
   sr = srunner_create(s);

   srunner_run_all(sr, CK_NORMAL);
   num_failed = srunner_ntests_failed(sr);
   srunner_free(sr);

   return (num_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;

}

/* endregion */