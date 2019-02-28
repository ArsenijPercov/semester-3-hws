#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

sem_t mutex, empty, full;
static int count = 0;

static void* produce(void* params){
   while(1){
      sem_wait(&empty);
      sem_wait(&mutex);
      count++;
      fprintf(stderr, "producer is producing: %d\n", count);
      sem_post(&mutex);
      sem_post(&full);
   }
}

static void* consume(void* params){
   while(1){
      sem_wait(&full);
      sem_wait(&mutex);
      count--;
      fprintf(stderr, "consumer is consuming %d\n", count);
      sem_post(&mutex);
      sem_post(&empty);
   }
}

int main(int argc, char* argv[]){

   int n, rc;
   pthread_t t1, t2;

   if(argc != 2){
      fprintf(stderr, "usage: ./main [buffer_size]\n");
      exit(EXIT_FAILURE);
   }

   n = atoi(argv[1]);

   sem_init(&mutex, 0, 1);
   sem_init(&empty, 0, n);
   sem_init(&full, 0, 0);

   pthread_create(&t1, NULL, produce, NULL);
   pthread_create(&t2, NULL, consume, NULL);
   pthread_join(t1, NULL);
   pthread_join(t2, NULL);


   sem_destroy(&mutex);
   sem_destroy(&full);
   sem_destroy(&empty);

   return 0;
}
