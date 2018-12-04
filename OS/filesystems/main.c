#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

int
main(int argc, char* argv[]){

   int rc;
   struct stat sb;
   char *buf;
   ssize_t bufsize, nbytes;

   if(argc != 2){
      fprintf(stderr, "usage: ./main [path]\n");
      exit(EXIT_FAILURE);
   }

   rc = lstat(argv[1], &sb);
   if(rc < 0){
      perror("lstat");
      exit(EXIT_FAILURE);
   }

   bufsize = sb.st_size + 1;
   if(sb.st_size == 0){
      bufsize = PATH_MAX;
   }

   buf = malloc(bufsize);
   if(buf == NULL){
      perror("malloc");
      exit(EXIT_FAILURE);
   }

   nbytes = readlink(argv[1], buf, bufsize);
   if(nbytes < 0){
      perror("readlink");
      exit(EXIT_FAILURE);
   }

   fprintf(stderr, "'%s' points to '%.*s'\n", argv[1], (int) nbytes, buf);
   if(bufsize == nbytes){
      fprintf(stderr, "Returned file might be turnicated\n");
   }

   free(buf);
   exit(EXIT_SUCCESS);
}
