/**
   @author Dushan Terzikj
   @brief This is the server-side of the program.

   Problem Sheet 5
   Operating Systems
   Prof. Juergen Schoenwaelder
 */

#define _POSIX_C_SOURCE 200809L
#define _DEFAULT_SOURCE

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>

#include "tcp.h"
#include "connections.h"
#include <event2/event.h>

#define CORRECT 0
#define INCORRECT 1
#define QUIT 2
#define CHALLENGE 3
#define UNKNOWN 4

static const char* MESSAGES[] = {
   "Congratulation - challenge passed!\n",
   "Wrong guess - expected: %s\n",
   "M: You mastered %d/%d challenges. Good bye!\n",
   "C: %s",
   "Unknown command or format\n"
};

static const char *progname = "guess-server";
static struct event_base *evb;
static conn_node *connections = NULL;
static char chars_POSIX[] = " \t\r\n\v\f.,;!~`_-"; /* POSIX whitespace and punctuation chars */


static void read_fortune(evutil_socket_t evfd, short evwhat, void *evarg);
static void get_fortune(conn_node* connection);
static void send_client(conn_node* connection, int type);
static void read_client(evutil_socket_t evfd, short evwhat, void *evarg);
static void set_connection(evutil_socket_t evfd, short evwhat, void *evarg);

int main(int argc, char* argv[]){

   int fds;
   struct event *tev;
   /* In case "localhost" does not work, NULL should work */
   const char* interfaces[] = {"localhost", NULL, "0.0.0.0", "::"};

   if(argc != 2){
      fprintf(stderr, "Not enough arguments. Usage: %s [port]\n", progname);
      exit(EXIT_FAILURE);
   }

   // this if for avoiding zombie processes
   if(signal(SIGCHLD, SIG_IGN) == SIG_ERR){
      fprintf(stderr, "%s: signal() %s\n", progname, strerror(errno));
      exit(EXIT_FAILURE);
   }

   (void) daemon(0, 0);
   openlog(progname, LOG_PID, LOG_DAEMON);
   evb = event_base_new();
   if(!evb){
      syslog(LOG_ERR, "creating event base failed");
      exit(EXIT_FAILURE);
   }

   for(int i = 0; interfaces[i]; i++){
      fds = tcp_listen(interfaces[i], argv[1]);
      if(fds < 0){
         fprintf(stderr, "error %s %s: could listen to connection\n", strerror(errno), progname);
         continue;
      }
      tev = event_new(evb, fds, EV_READ | EV_PERSIST, set_connection, NULL);
      event_add(tev, NULL);
   }

   if(event_base_loop(evb, 0) == -1){
      syslog(LOG_ERR, "event loop failed");
      event_base_free(evb);
      exit(EXIT_FAILURE);
   }

   closelog();
   event_base_free(evb);

   return EXIT_SUCCESS;
}

static void hide_word(state_t* curr_state){
   char *pos;
   char *token = NULL;
   int n;
   int word_pos;
   char tmp[64];

   pos = curr_state->fortune;
   while((pos = strpbrk(pos, chars_POSIX)) != NULL){
      pos++;
      n++;
   }

   while(token == NULL){

      srand(time(NULL));
      word_pos = rand()%n;
      n = 0;

      strcpy(tmp, curr_state->fortune);
      token = strtok(tmp, chars_POSIX);
      while(token){
         if(n == word_pos){
            pos = strstr(curr_state->fortune, token);
            if(!pos){
               break;
            }
            memset(pos, '_', strlen(token));
            strcpy(curr_state->missing, token);
            break;
         }
         n++;
         token = strtok(NULL, chars_POSIX);
      }
   }
}

static void read_fortune(evutil_socket_t evfd, short evwhat, void *evarg){
   int pfd = evfd;
   int nbytes, rc;
   conn_node *curr_conn = (conn_node*) evarg;
   char buf[64];

   nbytes = read(pfd, buf, 32);
   if(nbytes < 0){
      fprintf(stderr, "%s: read failed %s\n", progname, strerror(errno));
      exit(EXIT_FAILURE);
   } else {
      buf[nbytes] = '\0';
      strcpy(curr_conn->state->fortune, buf);
   }

   hide_word(curr_conn->state);
   send_client(curr_conn, CHALLENGE);
}

static void get_fortune(conn_node* connection){
   int pfd[2];
   pid_t pid;
   struct event *fev;

   (void) pipe(pfd);
   fev = event_new(evb, pfd[0], EV_READ, read_fortune, connection);
   event_add(fev, NULL);

   pid = fork();
   if(pid == -1){
      /* Failed forking */
      syslog(LOG_ERR, "failed to fork process\n");
      connection->state->fortune[0] = '\0';
   } else if(pid == 0){
      /* Child process */
      close(pfd[0]);
      dup2(pfd[1], STDOUT_FILENO);
      close(pfd[1]);

      execlp("fortune", "fortune", "-n", "32", "-s", NULL);
      /* If it comes here then execution has failed */
      syslog(LOG_ERR, "failed to execute fortune\n");
      exit(EXIT_FAILURE);
   } else {
      /* We only need the read endpoint so we close the write */
      close(pfd[1]);
   }
}

static void send_client(conn_node* connection, int type){
   char buf[256];
   int nbytes;
   if(type == QUIT){
      sprintf(buf, MESSAGES[QUIT], connection->state->correct, connection->state->num_guesses);
   } else if(type == CORRECT){
      strcpy(buf, MESSAGES[CORRECT]);
   } else if(type == INCORRECT){
      sprintf(buf, MESSAGES[INCORRECT], connection->state->missing);
   } else if(type == CHALLENGE){
      sprintf(buf, MESSAGES[CHALLENGE], connection->state->fortune);
   } else {
      strcpy(buf, MESSAGES[UNKNOWN]);
   }

   nbytes = tcp_write(connection->cfd, buf, strlen(buf));
   if(nbytes < 0){
      fprintf(stderr, "%s: write failed %s\n", progname, strerror(errno));
      exit(EXIT_FAILURE);
   }
}

static void read_client(evutil_socket_t evfd, short evwhat, void *evarg){
   int nbytes;
   int rc;
   int cfd = evfd;
   conn_node *curr_conn = (conn_node*) evarg;
   char buf[256];
   char *guess;

   nbytes = read(cfd, buf, sizeof(buf));
   if(nbytes < 0){
      fprintf(stderr, "%s: read failed: %s\n", progname, strerror(errno));
      exit(EXIT_FAILURE);
   }

   if(strstr(buf, "Q:") == buf){
      /* In case the user wants to quit */
      send_client(curr_conn, QUIT);
      event_free(curr_conn->cev);
      tcp_close(cfd);
      remove_conn_node(&connections, cfd);
   } else {
      if(strstr(buf, "R:") == buf){
         /* Response from the user */
         curr_conn->state->num_guesses++;
         memmove(buf, buf+2, strlen(buf)-2);
         guess = strtok(buf, chars_POSIX);
         if(guess == NULL || strcmp(guess, curr_conn->state->missing) != 0){
            send_client(curr_conn, INCORRECT);
         } else {
            curr_conn->state->correct++;
            send_client(curr_conn, CORRECT);
         }
         get_fortune(curr_conn);
      } else {
         /* Unrecognized command */
         send_client(curr_conn, UNKNOWN);
      }
   }
}

static void set_connection(evutil_socket_t evfd, short evwhat, void *evarg){
   size_t n;
   int cfd;
   char welcome[256];
   conn_node *new_conn;

   cfd = tcp_accept((int)evfd);
   if(cfd < 0){
      fprintf(stderr, "%s: could not connect: %s\n", progname, strerror(errno));
      exit(EXIT_FAILURE);
   }

   strcpy(welcome, "M: Guess the missing ____!\n");
   strcat(welcome, "M: Send your guess in the for 'R: word\\r\\n'.\n");

   n = tcp_write(cfd, welcome, strlen(welcome));
   if(n < 0){
      fprintf(stderr, "%s: %s writing failed\n", progname, welcome);
      tcp_close(cfd);
      exit(EXIT_FAILURE);
   }

   new_conn = add_new_conn_node(&connections, cfd);
   new_conn->cev = event_new(evb, cfd, EV_READ | EV_PERSIST, read_client, new_conn);
   event_add(new_conn->cev, NULL);

   get_fortune(new_conn);
}

