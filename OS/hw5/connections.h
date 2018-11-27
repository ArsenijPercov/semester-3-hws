#ifndef _CONNECTIONS_H
#define _CONNECTIONS_H

#include <event2/event.h>

typedef struct state {
   int num_guesses;
   int correct;
   char fortune[33];
   char missing[33];
} state_t;

typedef struct node {
   int cfd;
   state_t *state;
   struct event *cev;
   struct node *next;
} conn_node;

int remove_conn_node(conn_node **head, int cfd);
conn_node* add_new_conn_node(conn_node **head, int cfd);

#endif
