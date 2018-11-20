#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[]){

  pid_t pid;
  int rc, fds[2];
  char buf[20];

  rc = pipe(fds);
  if(rc == -1){
    perror("pipe");
    exit(EXIT_FAILURE);
  }
  
  pid = fork();
  if(pid == -1){
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if(pid == 0){
    /* Child process */
    fprintf(stderr, "Child Process\n");
    close(fds[0]);
    write(fds[1], "Hello World!", 12);
  } else {
    /* Parent process */
    fprintf(stderr, "Parent Process\n");
    close(fds[1]);
    read(fds[0], buf, 12);
    fprintf(stderr, "buf: %s", buf);
  }
  
  return 0;
}
