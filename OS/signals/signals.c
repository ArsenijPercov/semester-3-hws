#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

volatile sig_atomic_t keep_going = 1;

static const char* message = "interrupted";
static int m_len;

static void handler(int signum){
   keep_going = 0;
   write(STDOUT_FILENO, message, m_len);
}

int main(int argc, char* argv[]){

   signal(SIGINT, handler);
   char buf;
   m_len = strlen(message);

   while(keep_going){
      read(STDIN_FILENO, &buf, 1);
      write(STDOUT_FILENO, &buf, 1);
   }

   return 0;
}
