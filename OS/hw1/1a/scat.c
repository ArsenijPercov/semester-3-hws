/**
 * Author: Dushan Terzikj
 * Assignment: 1
 * Problem: 1a
 * Course: Operating Systems
 * Professor: Juergen Schoenwaelder
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define LIB_CALL 0
#define SYS_CALL 1

int main(int argc, char* argv[]){

    int flag = -1, opt;
    /* Flagging the indicator: should it be library or system call */
    while((opt = getopt(argc, argv, "ls")) != -1){
        switch(opt){
            case 'l':
            flag = LIB_CALL;
            break;
            case 's':
            flag = SYS_CALL;
            break;
            default:
            flag = LIB_CALL;
        }
    }
    if(flag == -1){
        /* Command lines error */
        exit(EXIT_FAILURE);
    }

    char c;
    if(flag == LIB_CALL){
        /* Using lib calls */
        while((c = getc(stdin)) != EOF){
            if(putc(c, stdout) == EOF){
                /* Error on printing */
                exit(EXIT_FAILURE);
            }
        }
    } else {
        /* Using sys calls */
        ssize_t nc;
        while((nc = read(0, &c, 1)) > 0){
            nc = write(1, &c, 1);
            if(nc < 0){
                /* Error writing */
                exit(EXIT_FAILURE);
            }
        }
        if(nc < 0){
            /* Error reading */
            exit(EXIT_FAILURE);
        }
    }

    return EXIT_SUCCESS;
}