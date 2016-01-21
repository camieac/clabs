#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <time.h>

void * dummy (void * arg)
{
   return arg;
}
/*	Returns the time between time a and b.
 */ 
double xelapsed (struct timespec a, struct timespec b)
{
   return (a.tv_sec - b.tv_sec) * 1000000.0
          + (a.tv_nsec - b.tv_nsec) / 1000.0;
}
/*
 * Parameter n is the number of forks to create.
 */ 
void measure_fork (int n)
{
   struct timespec start, stop, finish;

   clock_gettime (CLOCK_REALTIME, &start);
   /*
    * Make N forks, call dummy in every child.
    */
    
	pid_t stored_id [n]; //Stores the PID for each fork (child process).
	int i;
	for(i = 0; i < n; i++){

      pid_t pid;
    
      pid = fork();
      if (pid < 0)//Fork failed, exit with failure status
      {
        printf("fork failed!\n");
        exit(EXIT_FAILURE); //Return failure in exit status
      }
      
      
      if (pid > 0)//Kill the parent process
      {
        exit(EXIT_SUCCESS);//Return success in exit status
      }
	  stored_id[i] = pid;
	
	  dummy (&i);
	}
   clock_gettime (CLOCK_REALTIME, &stop);
   /*
    * Wait for the forks.
    * Wait for a state change in each child process.
    * We assume we get a child termination as the state change.
    */
    int j;
    for (j = 0; j < n; j++){
		wait(&(stored_id[j])); 
	}
   clock_gettime (CLOCK_REALTIME, &finish);

   printf ("%d proc: fork=%5.0f (mics) wait=%5.0f (mics) sum=%5.0f (mics)\n",
           n, xelapsed (stop, start),
           xelapsed (finish, stop), xelapsed (finish, start));
   printf ("per proc: fork=%5.0f (mics) wait=%5.0f (mics) sum=%5.0f (mics)\n",
           xelapsed (stop, start)/n,
           xelapsed (finish, stop)/n, xelapsed (finish, start)/n);
}

int main(int argc, char*argv[])
{
   int n;

   if( argc == 2) {
     n = atoi( argv[1]);
     measure_fork(n);
  } else {
     int arr[12] = {1,2,4,8,16,32,64,128,256,512,1024,10000};
     int i = 0;
     for(i = 0; i < sizeof(arr);i++){
		 measure_fork(arr[i]);
	 }
  }
  

  return(0);
}
