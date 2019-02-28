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
   struct _rpn_stack *prev;
} rpn_stack_t;

rpn_stack_t* rpn_stack_new() {
   rpn_stack_t *s = malloc(sizeof(rpn_stack_t));
   if(s == NULL){
      perror("malloc: rpn_stack_new");
      exit(EXIT_FAILURE);
   }
   s->data = NULL;
   s->prev = NULL;
   return s;
}

void rpn_stack_push(rpn_stack_t *s, void *data) {
   rpn_stack_t *nel = malloc(sizeof(rpn_stack_t));
   if(nel == NULL){
      perror("malloc: rpn_stack_push");
      exit(EXIT_FAILURE);
   }
   nel->data = data;
   nel->prev = s->prev;
   s->prev = nel;
}

void* rpn_stack_pop(rpn_stack_t *s) {

   rpn_stack_t *tmp = s->prev;
   void *data = tmp->data;
   s->prev = tmp->prev;
   tmp->prev = NULL;
   free(tmp);
   return data;
}

int rpn_stack_empty(rpn_stack_t* s) {
   return (s->prev == NULL) ? 1 : 0;
}

void* rpn_stack_peek(rpn_stack_t *s) {
   if(rpn_stack_empty(s) == 0){
      return s->prev->data;
   } else {
      return NULL;
   }
}

void rpn_stack_del(rpn_stack_t *s) {
   while(rpn_stack_empty(s) == 0){
      (void) rpn_stack_pop(s);
   }
}
