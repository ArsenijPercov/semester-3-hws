#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void my_sig_handler(int signum){
  fprintf(stderr, "signal handled with code - %d", signum);

  signal(SIGTERM, my_sig_handler);
}

int main(int argc, char* argv[]){

  int rc, i = 0;

  signal(SIGTERM, my_sig_handler);
  
  while(1){
    fprintf(stderr, "i = %d", i);
    i++;
    sleep(1);
  }

  return 0;
}
