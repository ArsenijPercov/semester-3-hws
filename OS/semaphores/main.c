#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

sem_t sem;
static int pnum[] = {1, 2};
static int count = 0;

static void* proc(void* params){
   int *ptr = (int*) params;
   int i = *ptr;
   while(1){
      sem_wait(&sem);
      fprintf(stderr, "Process %d accessing the critical section\n", i);
      fprintf(stderr, "Before: count=%d\n", count);
      if(i == 1)
         count++;
      else
         count--;
      fprintf(stderr, "After: count=%d\n", count);
      usleep(rand()%2000000 + 1000000);
      sem_post(&sem);
      sleep(1);
   }
}

int main(int argc, char* argv[]){

   sem_init(&sem, 0, 1);
   pthread_t t1, t2;
   pthread_create(&t1, NULL, proc, &pnum[0]);
   pthread_create(&t2, NULL, proc, &pnum[1]);

   pthread_join(t1, NULL);
   pthread_join(t2, NULL);

   sem_destroy(&sem);

   return 0;
}
