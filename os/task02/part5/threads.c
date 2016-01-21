#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM_THREADS	3

//bintree* tree = (bintree *)malloc(sizeof(bintree));


void *doLoop(void *data)
{
	printf("Thread Running\n");
  
int *x = (int *) data;
  
 
   int i;
	for(i = 0; i < 3; i++){
		int temp = *x +1;
		//sleep(1);
		*x = temp;
	}

   //printf("X after a thread: %i",x);
   pthread_exit(NULL);
   printf("Thread Terminated\n");
}

int main(int argc, char *argv[])
{
pthread_t threads[NUM_THREADS];
int rc;
int x = 0;
int *data = &x;
int t;

for(t=0;t<NUM_THREADS;t++) {
  printf("Creating thread %d\n", t);
  //printf("X before thread: %i",x);
  rc = pthread_create(&threads[t], NULL, doLoop, (void *) 
       data);
  if (rc) {
    printf("ERROR; return code from pthread_create() is %d\n", rc);
    exit(-1);
    }
  }
  
  sleep(1);
  printf("Final Value for x: %i\n",x);
pthread_exit(NULL);
}
