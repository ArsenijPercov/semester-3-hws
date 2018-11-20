#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>

int main(int argc, char* argv[]){

  int rc, fd;
  char buf[12];
  
  fd = open("./mypipe", O_RDONLY);
  if(fd == -1){
    perror("open");
    exit(EXIT_FAILURE);
  }

  rc = read(fd, buf, 12);
  if(rc == -1){
    perror("read");
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "%s\n", buf); 
  
  return 0;
}
