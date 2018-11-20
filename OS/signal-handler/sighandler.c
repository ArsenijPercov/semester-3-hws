#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

static volatile int keep_going = 1;

void catch_signal(int signum){
  keep_going = 0;
  fprintf(stderr, "Interrupted!\n");
}

int main(int argc, char* argv[]){
  signal(SIGINT, catch_signal);

  int c, rc = 0;
  
  while(keep_going == 1){
    rc = read(STDIN_FILENO, &c, 1);
    if(rc == -1){
      fprintf(stderr, "error\n");
      exit(EXIT_FAILURE);
    }
    rc = write(STDOUT_FILENO, &c, 1);
    if(rc == -1){
      fprintf(stderr, "write error\n");
      exit(EXIT_FAILURE);
    }
  }

  return (rc == 0);
}
