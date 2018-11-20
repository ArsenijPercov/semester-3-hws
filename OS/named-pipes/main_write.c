#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char* argv[]){

  int rc;
  char buf[20];
  int fd;

  rc = mkfifo("./mypipe", 0666);
  if(rc == -1){
    perror("mkfifo");
    exit(EXIT_FAILURE);
  }

  fd = open("./mypipe", O_WRONLY);
  if(fd == -1){
    perror("open");
    exit(EXIT_FAILURE);
  }

  write(fd, "Hello World!", 12);
  rc = close(fd);

  if(rc == -1){
    perror("close");
    exit(EXIT_FAILURE);
  }
  
  return 0;
}
