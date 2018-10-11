/**
 * @author  Dushan Terzkj
 *
 * Assignment 2
 * Problem 1
 *
 * Operating Systems CO20-320202
 * Prof. Juergen Schoenwaelder
 *
 */
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>

#define null NULL

typedef unsigned int ui;

ui num_persons = 100;
ui num_flips = 10000;
static char coins[20] = "OOOOOOOOOOXXXXXXXXXX";
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static int idx = 0;
static const char *progname = "pthread";

/**
 * Flips the coin.
 *
 * @param   c   The current coin side.
 * @return  char    The other side of the coin.
 */
char flip(char c){
    if(c == 'O'){
        return 'X';
    }
    return 'O';
}

/**
 * Calls a function which was assigned to a specific thread/process.
 * This code has been copied from Prof. Schoenwaelder source code directories.
 *
 * @param   n   Data which is used for the process.
 * @param   f   The function which is being called.
 */
static void run_threads(ui n, void* (*proc)(void*))
{
    pthread_t *thread;
    int rc;

    thread = calloc(n, sizeof(pthread_t));
    if (! thread) {
        fprintf(stderr, "%s: %s: %s\n", progname, __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        rc = pthread_create(&thread[i], NULL, proc, NULL);
        if (rc) {
            fprintf(stderr, "%s: %s: unable to create thread %d: %s\n",
                    progname, __func__, i, strerror(rc));
        }
    }

    for (int i = 0; i < n; i++) {
        if (thread[i]) {
            (void) pthread_join(thread[i], NULL);
        }
    }

    (void) free(thread);
}


/**
 * Times a process.
 *
 * @param   n       Data about arguments which will later be used in the processes.
 * @param   proc    The process which needs to be run.
 * @return  double  The time idx took for the process to finish.
 */
static double timeit(ui n, void* (*f)(void *)){
    clock_t t1, t2;
    t1 = clock();
    run_threads(n, f);
    t2 = clock();
    return (((double)(t2-t1))/CLOCKS_PER_SEC)*1000.0;
}

/**
 * Function which flows like the first strategy explained in the problem.
 *
 * @param   data
 * @return  null
 */
static void *first_strategy(void *data){

    pthread_mutex_lock(&mutex);
    for(int i = 0; i < num_flips; i++){
        for(int j = 0; j < 20; j++){
            coins[j] = flip(coins[j]);
        }
    }
    pthread_mutex_unlock(&mutex);
    return null;
}

/**
 * Function which flows like the second strategy explained in the problem.
 *
 * @param   data
 * @return  null
 */
static void *second_strategy(void *data){

    for(int i = 0; i < num_flips; i++){
        pthread_mutex_lock(&mutex);
        for(int j = 0; j < 20; j++){
            coins[j] = flip(coins[j]);
        }
        pthread_mutex_unlock(&mutex);
    }
    return null;
}

/**
 * Function which flows like the third strategy explained in the problem.
 *
 * @param   data
 * @return  null
 */
static void *third_strategy(void *data){
    for(int i = 0; i < num_flips; i++){
        for(int j = 0; j < 20; j++){
            pthread_mutex_lock(&mutex);
            coins[j] = flip(coins[j]);
            pthread_mutex_unlock(&mutex);
        }
    }
    return null;
}

int main(int argc, char* argv[]) {

//    coins = (char*)malloc(20* sizeof(char));
//    if(!coins){
//        perror("error: unable to allocate array\n");
//        return EXIT_FAILURE;
//    }
//    /* Filling the coins string */
//    for(int i = 0; i < 10; i++){
//        coins[i] = 'O';
//        coins[i+10] = 'X';
//    }
    int opt;
    /* Read arguments from shell command line */
    while((opt = getopt(argc, argv, "p:n:")) != -1){
        switch (opt){
            case 'p':
                num_persons = (ui) atoi(optarg);
                if(num_persons <= 0) {
                    perror("error: invalid number of persons\n");
                    return EXIT_FAILURE;
                }
                break;
            case 'n':
                num_flips = (ui) atoi(optarg);
                if(num_flips <= 0){
                    perror("error: invalid number of flips\n");
                    return EXIT_FAILURE;
                }
                break;
            default:
                perror("error: invalid command\n");
                return EXIT_FAILURE;
        }
    }

    /* Calling the first strategy */
    printf("coins: %s (start - global lock)\n", coins);
    double t1 = timeit(num_persons, first_strategy);
    printf("coins: %s (end - global lock)\n", coins);
    printf("%d threads x %d flips: %.3lf ms\n\n", num_persons, num_flips, t1);

    /* Calling the second strategy */
    printf("coins: %s (start - iteration lock)\n", coins);
    double t2 = timeit(num_persons, second_strategy);
    printf("coins: %s (end - iteration lock)\n", coins);
    printf("%d threads x %d flips: %.3lf ms\n\n", num_persons, num_flips, t2);

    /* Calling the third strategy */
    printf("coins: %s (start - coin lock)\n", coins);
    double t3 = timeit(num_persons, third_strategy);
    printf("coins: %s (end - coin lock)\n", coins);
    printf("%d threads x %d flips: %.3lf ms\n\n", num_persons, num_flips, t3);

    return 0;
}