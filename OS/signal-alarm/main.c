#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

static int ticks = 0;

static void handler(int signum){
  if(++ticks > 5){
    raise(SIGTERM);
  }
  alarm(1);
}

int main(int argc, char* argv[]){
  struct sigaction sa = { .sa_handler = handler, .sa_flags = 0 };
  sigaction(SIGALRM, &sa, NULL);
  alarm(1);
  while(1){
    sigset_t s;
    char c;
    read(STDIN_FILENO, &c, 1);
    printf("tick tock: %d\n", ticks);
  }
  return 0;
}
