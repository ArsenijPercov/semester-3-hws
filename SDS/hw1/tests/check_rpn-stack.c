/*
hw1/tests/check_rpn-stack.c

Author: Dushan Terzikj
 */

#include <stdlib.h>
#include <stdio.h>
#include <check.h>

#include "rpn-stack.h"

/* region GLOBAL IMPLEMENTATIONS */

/**
 * \brief   Implementation of the stack. In this stack, the bottommost 
 *          element is the first element of the linked list. The top
 *          most element is the head of the stack
 *          [] --> [] --> [] --> NULL
 *           \             \
 *           bottom        top
*/
typedef struct _rpn_stack {
   void *data;
   struct _rpn_stack *prev;
} rpn_stack_t;

/* endregion */

/* region TESTS */

/**
 * \brief   Test whether a new stack is successfully allocated.
 *          Create a new stack and check whether it's not null.
*/
START_TEST(test_new_stack)
{
   fprintf(stderr, "Running test_new_stack...\n");
   rpn_stack_t *s = rpn_stack_new();
   ck_assert_ptr_ne(s, NULL);
   fprintf(stderr, "Done: test_new_stack\n");
}
END_TEST

/**
 * \brief   Test whether the push function works. Create a copy pointer
 *          to the same stack and just keep track whether the data added
 *          pointer is the same as the data pointer.
*/
START_TEST(test_push_stack)
{
   fprintf(stderr, "Running test_push_stack...\n");
   rpn_stack_t *s = rpn_stack_new();
   char tests[] = {'a', 'B', '1', '!', '3', '-'};
   char *pt = tests;
   rpn_stack_t *tmp; // another pointer to the same stack
   size_t len = sizeof(tests)/sizeof(tests[0]);
   size_t i;
   for(i = 0; i < len; i++){
      rpn_stack_push(s, pt);
      tmp = s->prev;
      ck_assert_ptr_eq(tmp->data, pt);
      pt += sizeof(tests[0]);
   }
   fprintf(stderr, "Done: test_push_stack\n");
}
END_TEST

/*
  \brief Test case that tests whether the empty function is working
*/
START_TEST(test_empty_stack)
{
   // TODO: Write test cases
   fprintf(stderr, "Running test_empty_stack...\n");
   rpn_stack_t *s = rpn_stack_new();
   ck_assert_int_eq(rpn_stack_empty(s), 1);
   rpn_stack_push(s, (void*)1);
   ck_assert_int_eq(rpn_stack_empty(s), 0);
   fprintf(stderr, "Done: test_empty_stack\n");
}
END_TEST

/*
  \brief Test case that tests whether the peek function works. Check
         if the latest added element is on the top of the stack.
*/
START_TEST(test_peek_stack)
{
   fprintf(stderr, "Running: test_peek_stack...\n");

   rpn_stack_t *s = rpn_stack_new();
   ck_assert_ptr_eq(rpn_stack_peek(s), NULL);

   int tests[] = {1, 2, 3, 4, 5};
   int *pt = tests;
   size_t len = sizeof(tests)/sizeof(tests[0]);
   size_t i;
   for(i = 0; i < len; i++){
      rpn_stack_push(s, pt);
      ck_assert_ptr_eq(rpn_stack_peek(s), pt);
      pt += sizeof(tests[0]);
   }

   fprintf(stderr, "Done: test_peek_stack.\n");
}
END_TEST

/*
  \brief Test case that tests whether the pop function works. The last 
         added element should be the first popped element.
*/
START_TEST(test_pop_stack)
{
   fprintf(stderr, "Running test_pop_stack...\n");
   rpn_stack_t *s = rpn_stack_new();
   char tests[] = {'a', 'B', '1', '!', '3', '-'};
   char *pt = tests;
   size_t len = sizeof(tests)/sizeof(tests[0]);
   size_t i;
   void *ret_val;
   for(i = 0; i < len; i++){
      rpn_stack_push(s, pt);
      pt += sizeof(tests[0]);
   }
   pt -= sizeof(tests[0]);
   for(int i = 0; i < len; i++){
      ret_val = rpn_stack_pop(s);
      ck_assert_ptr_eq(ret_val, pt);
      pt -= sizeof(tests[0]);
   }
   fprintf(stderr, "Done: test_pop_stack\n");
}
END_TEST

/**
 * \brief   Test whether delete function works. Create a stack, maybe
 *          push some data in it, then call the delete method and 
 *          check whether the stack is empty.
*/
START_TEST(test_del_stack)
{
   fprintf(stderr, "Running test_del_stack...\n");

   rpn_stack_t *s1 = rpn_stack_new();
   rpn_stack_del(s1);
   ck_assert_int_eq(rpn_stack_empty(s1), 1);

   rpn_stack_t *s2 = rpn_stack_new();
   rpn_stack_push(s2, (void*)1);
   rpn_stack_push(s2, (void*)1);
   rpn_stack_del(s2);
   ck_assert_int_eq(rpn_stack_empty(s2), 1);

   fprintf(stderr, "Done: test_del_stack\n");
}
END_TEST

/* endregion */

/* region SUITE */

static Suite* gdb_suite(void) {

   Suite* s;
   TCase *tc_empty_stack, *tc_new_stack, *tc_pop_stack, *tc_push_stack, 
   *tc_peek_stack, *tc_del_stack;

   // Create suite and test cases
   s = suite_create("all");
   tc_empty_stack = tcase_create("empty_stack");
   tc_new_stack = tcase_create("new_stack");
   tc_pop_stack = tcase_create("pop_stack");
   tc_push_stack = tcase_create("push_stack");
   tc_peek_stack = tcase_create("peek_stack");
   tc_del_stack = tcase_create("del_stack");

   // Add test cases to suite
   tcase_add_test(tc_empty_stack, test_empty_stack);
   suite_add_tcase(s, tc_empty_stack);

   tcase_add_test(tc_new_stack, test_new_stack);
   suite_add_tcase(s, tc_new_stack);

   tcase_add_test(tc_pop_stack, test_pop_stack);
   suite_add_tcase(s, tc_pop_stack);

   tcase_add_test(tc_push_stack, test_push_stack);
   suite_add_tcase(s, tc_push_stack);

   tcase_add_test(tc_peek_stack, test_peek_stack);
   suite_add_tcase(s, tc_peek_stack);

   tcase_add_test(tc_del_stack, test_del_stack);
   suite_add_tcase(s, tc_del_stack);

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