#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "connections.h"

conn_node* add_new_conn_node(conn_node **head, int cfd){
   conn_node *curr_node = NULL;
   state_t *curr_state = NULL;

   if(*head == NULL){
      *head = (conn_node*) malloc(sizeof(conn_node));
      if(*head == NULL){
         fprintf(stderr, "error creating new connection: %s", strerror(errno));
         exit(EXIT_FAILURE);
      }
      curr_node = *head;
   } else {
      curr_node = *head;
      while(curr_node->next != NULL){
         curr_node = curr_node->next;
      }
      curr_node->next = (conn_node*) malloc(sizeof(conn_node));
      curr_node = curr_node->next;
      curr_node->next = NULL;
   }
   curr_node->cfd = cfd;
   curr_node->cev = NULL;
   curr_node->state = (state_t*) malloc(sizeof(state_t));

   curr_state = curr_node->state;
   curr_state->num_guesses = 0;
   curr_state->correct = 0;
   curr_state->fortune[0] = '\0';
   curr_state->missing[0] = '\0';

   return curr_node;
}

int remove_conn_node(conn_node **head, int cfd){
   conn_node *curr_node = NULL;
   if(*head == NULL){
      fprintf(stderr, "the connection list is empty\n");
      return EXIT_FAILURE;
   }

   curr_node = *head;
   if(curr_node->cfd == cfd){
      *head = ((*head)->next) ? (*head)->next : NULL;
      event_free(curr_node->cev);
      free(curr_node);
      return EXIT_SUCCESS;
   }

   while(curr_node->next != NULL){
      if(curr_node->next->cfd == cfd){
         conn_node* tmp = curr_node->next;
         curr_node->next = (curr_node->next->next) ? curr_node->next->next : NULL;
         event_free(tmp->cev);
         free(tmp);
         break;
      }
      curr_node = curr_node->next;
   }

   return EXIT_SUCCESS;
}
