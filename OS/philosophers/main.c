#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>



enum {THINKING, HUNGRY, EATING} state[5];
static int n;
static int phil[] = {0, 1, 2, 3, 4};
static sem_t mutex;
static sem_t p[5];

static void* proc(void*);
static void think(int i);
static void take_forks(int i);
static void put_forks(int i);
static void eat(int i);
static void test(int i);

int main(int argc, char* argv[]){

   int rc;

   /*  if(argc != 2){ */
   /*    fprintf(stderr, "usage: ./main [num_philosophers]\n"); */
   /*    exit(EXIT_FAILURE); */
   /* } */

   //n = atoi(argv[i]);
   srand(time(NULL));
   n = 5;
   sem_init(&mutex, 0, 1);
   for(int i = 0; i < n; i++){
      sem_init(&p[i], 0, 0);
   }
   pthread_t *threads;
   threads = calloc(n, sizeof(pthread_t));
   for(int i = 0; i < n; i++){
      rc = pthread_create(&threads[i], NULL, proc, &phil[i]);
      if(rc){
         perror("pthread");
      }
   }

   for(int i = 0; i < n; i++){
      if(threads[i]){
         pthread_join(threads[i], NULL);
      }
   }
   sem_destroy(&mutex);
   for(int i = 0; i < n; i++){
      sem_destroy(&p[i]);
   }
}

static void* proc(void* params){
   int* ptr = (int*) params;
   int i = *ptr;
   while(1){
      think(i);
      take_forks(i);
      eat(i);
      put_forks(i);
   }
}

static void test(int i){
   if(state[i] == HUNGRY
      && state[(i+4)%n] != EATING
      && state[(i+1)%n] != EATING){
      state[i] = EATING;
      fprintf(stderr, "philosopher %d takes forks %d and %d\n", i, (i+4)%n, i);
      sem_post(&p[i]);
   }
}

static void take_forks(int i){
   sem_wait(&mutex);
   state[i] = HUNGRY;
   test(i);
   sem_post(&mutex);
   sem_wait(&p[i]);
}

static void put_forks(int i){
   sem_wait(&mutex);
   state[i] = THINKING;
   fprintf(stderr, "philosopher %d puts down the forks %d and %d\n", i, (i+4)%n, i);
   test((i+1)%n);
   test((i+4)%n);
   sem_post(&mutex);
}

static void eat(int i){
   fprintf(stderr, "philosopher %d is eating\n", i);
   usleep(rand()%5000000 + 1000000);
}

static void think(int i){
   fprintf(stderr, "philosopher %d is thinking\n", i);
   usleep(rand()%5000000 + 1000000);
}
