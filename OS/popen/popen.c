#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){

   FILE *file = popen("ls", "r");
   char buf[128];
   read(fileno(file), &buf, 128);
   pclose(file);
   write(STDOUT_FILENO, &buf, strlen(buf));

   return EXIT_SUCCESS;
}
