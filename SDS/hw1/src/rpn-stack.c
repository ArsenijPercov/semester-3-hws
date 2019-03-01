/**
 * h1/src/rpn-stack.c
 * 
 * Author: Dushan Terzikj
*/

#include <stdlib.h>
#include <stdio.h>

#include "rpn-stack.h"

/* Implementation of the stack */
typedef struct _rpn_stack {
   void *data;
   struct _rpn_stack *next;
} rpn_stack_t;

rpn_stack_t* rpn_stack_new() {
   // TODO: Implement
   rpn_stack_t *s;
   s = (rpn_stack_t*) malloc(sizeof(rpn_stack_t));
   if(s == NULL){
       perror("malloc");
       exit(EXIT_FAILURE);
   }
   s->data = NULL;
   s->next = NULL;
   return s;
}

void rpn_stack_push(rpn_stack_t *s, void *data) {
   // TODO: Implement
    rpn_stack_t *tmp;
    tmp = (rpn_stack_t*) malloc(sizeof(rpn_stack_t));
    if(tmp == NULL || s == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    tmp->data = data;
    tmp->next = s->next;
    s->next = tmp;
}

void* rpn_stack_pop(rpn_stack_t *s) {

    rpn_stack_t *tmp;
    void *ret;

    tmp = s->next;
    ret = tmp->data;
    s->next = tmp->next;
    tmp->next = NULL;
    free(tmp);
    return ret;
}

int rpn_stack_empty(rpn_stack_t* s) {
    return (s->next == NULL) ? 1 : 0;
}

void* rpn_stack_peek(rpn_stack_t *s) {
    if(rpn_stack_empty(s)){
        return NULL;
    } else {
        return s->next->data;
    }
}

void rpn_stack_del(rpn_stack_t *s) {
    while(!rpn_stack_empty(s)){
        (void) rpn_stack_pop(s);
    }
}

void rpn_print(rpn_stack_t *s) {
    rpn_stack_t *curr;
    curr = s->next;
    while(curr != NULL){
        printf("%d ", *((int*)curr->data));
        curr = curr->next;
    }
    printf("\n");
}