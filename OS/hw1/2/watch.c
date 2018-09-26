/**
 * Author: Dushan Terzikj
 * Assignment: 1
 * Problem: 2
 * Course: Operating Systems
 * Professor: Juergen Schoenwaelder
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <getopt.h>
#include <sys/types.h>

#define TRUE 1
#define FALSE 0

static int invalid_options = 0;

int watch(int argc, char* argv[], int sleep_time, int fail_exec_stop, int special_value){

    optind -= invalid_options;

    while(TRUE){
        pid_t pid = fork();
        if(pid < 0){
            perror("error: creating a child process failed\n");
            exit(-1);
        } else if(pid == 0){
            char *new_argv[argc-optind+1];
            for(int i = 0; i < argc-optind; i++){
                new_argv[i] = argv[argc-1-i];
            }
            new_argv[argc-optind] = '\0';

            if(execvp(new_argv[0], new_argv) == -1){
                if(special_value){
                    perror("a\n");
                }
                perror("error: runtime error\n");
                _exit(EXIT_FAILURE);
            }
            _exit(EXIT_SUCCESS);
        } else {

            int wait_process_status;
            (void) waitpid(-1, &wait_process_status, WUNTRACED);

            if(fail_exec_stop && wait_process_status){
                exit(EXIT_FAILURE);
            }
            (void) sleep(sleep_time);
        }
    }

    return EXIT_FAILURE;
}

int main(int argc, char* argv[]){

    int sleep_time = 2;
    int opt, fail_exec_stop = 0;
    int special_value = 0;

    while((opt = getopt(argc, argv, "n:be")) != -1){
        switch(opt){
            case 'n':
                sleep_time = atoi(optarg);
                if(sleep_time < 1){
                    perror("error: invalid time value\n");
                    exit(EXIT_FAILURE);
                }
            break;
            case 'b':
                special_value = 1;
            break;
            case 'e':
                fail_exec_stop = 1;
            break;
            default:
                invalid_options++;
        }
    }

    if(watch(argc, argv, sleep_time, fail_exec_stop, special_value)){
        perror("error: runtime error\n");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}