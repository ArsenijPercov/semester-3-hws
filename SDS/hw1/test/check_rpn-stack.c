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
   struct _rpn_stack *next;
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
   rpn_stack_t *s;
   s = rpn_stack_new();
   ck_assert_ptr_ne(s, NULL);
   fprintf(stderr, "Done\n----------------------\n");
}
END_TEST

/**
 * \brief   Test whether the push function works. Push an element into the stack,
 *          take a peek and see if the elements are equal. 
*/
START_TEST(test_push_stack)
{

    printf("Running test_push_stack...\n");

   rpn_stack_t *s;
   s = rpn_stack_new();
   ck_assert_int_eq(rpn_stack_empty(s), 1); // check if the stack is initially empty
   int test[] = {1, 2, 3, 4, 5};
   size_t len = sizeof(test)/sizeof(test[0]);
   size_t i;
   void *tmp;
   for(i = 0; i < len; i++){
        rpn_stack_push(s, &test[i]);
        tmp = rpn_stack_peek(s);
        ck_assert_int_eq(test[i], *((int*)tmp));
   }
   ck_assert_int_eq(rpn_stack_empty(s), 0); // stack should not be empty now
   
   /* Check what happens with pop */
   (void) rpn_stack_pop(s);
   tmp = rpn_stack_peek(s);
   ck_assert_int_eq(test[len-2], *((int*)tmp));

    printf("Done\n----------------------------\n");

}
END_TEST

/*
  \brief Test case that tests whether the empty function is working
*/
START_TEST(test_empty_stack)
{

    printf("Running: test_empty_stack...\n");

    rpn_stack_t *s;
    s = rpn_stack_new();
    const int emp = 1;
    int tmp = 5;
    ck_assert_int_eq(rpn_stack_empty(s), emp);
    rpn_stack_push(s, &tmp);
    rpn_stack_push(s, &tmp);
    ck_assert_int_eq(rpn_stack_empty(s), !emp);
    (void) rpn_stack_pop(s);
    ck_assert_int_eq(rpn_stack_empty(s), !emp);
    (void) rpn_stack_pop(s);
    ck_assert_int_eq(rpn_stack_empty(s), emp);

    printf("Done\n----------------------------------\n");
}
END_TEST

/*
  \brief Test case that tests whether the peek function works. Check
         if the latest added element is on the top of the stack.
*/
START_TEST(test_peek_stack)
{
    printf("Running test_peek_stack...\n");

   rpn_stack_t *s;
   s = rpn_stack_new();
   int test[] = {1, 2, 3, 4, 5};
   size_t len = sizeof(test)/sizeof(test[0]);
   size_t i;
   void *tmp;
   for(i = 0; i < len; i++){
        rpn_stack_push(s, &test[i]);
        tmp = rpn_stack_peek(s);
        ck_assert_int_eq(test[i], *((int*)tmp));
   }
   
   /* Check what happens with pop */
   (void) rpn_stack_pop(s);
   tmp = rpn_stack_peek(s);
   ck_assert_int_eq(test[len-2], *((int*)tmp));

    printf("Done\n----------------------------\n");
}
END_TEST

/*
  \brief Test case that tests whether the pop function works. The last 
         added element should be the first popped element.
*/
START_TEST(test_pop_stack)
{
    printf("Running test_pop_stack...\n");

    rpn_stack_t *s;
    s = rpn_stack_new();
    ck_assert_int_eq(rpn_stack_empty(s), 1); // check if the stack is initially empty
    int test[] = {1, 2, 3, 4, 5};
    size_t len = sizeof(test)/sizeof(test[0]);
    int i;
    void *tmp;
    for(i = 0; i < (int) len; i++){
        rpn_stack_push(s, &test[i]);
    }
    ck_assert_int_eq(rpn_stack_empty(s), 0); // stack should not be empty now

    for(i = len-1; i >= 0; i--){
        tmp = rpn_stack_pop(s);
        ck_assert_int_eq(test[i], *((int*)tmp));
    }

    printf("Done\n----------------------------\n");
}
END_TEST

/**
 * \brief   Test whether delete function works. Create a stack, maybe
 *          push some data in it, then call the delete method and 
 *          check whether the stack is empty.
*/
START_TEST(test_del_stack)
{

    printf("Running: test_del_stack...\n");

    rpn_stack_t *s;
    const int emp = 1;
    int tmp = 5;
    s = rpn_stack_new();
    ck_assert_int_eq(rpn_stack_empty(s), emp);
    rpn_stack_push(s, &tmp);
    rpn_stack_push(s, &tmp);
    ck_assert_int_eq(rpn_stack_empty(s), !emp);
    rpn_stack_del(s);
    ck_assert_int_eq(rpn_stack_empty(s), emp);

    printf("Done\n----------------------------\n");
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