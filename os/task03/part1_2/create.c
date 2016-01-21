  #include <stdlib.h>
  #include <stdio.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <sys/time.h>
  #include <time.h>
  #include <pthread.h>
 
void * dummy (void * arg)
{
 return arg;
}
 
double xelapsed (struct timespec a, struct timespec b)
{
 return (a.tv_sec - b.tv_sec) * 1000000.0
 + (a.tv_nsec - b.tv_nsec) / 1000.0;
}
 
void measure_fork (int n)
{
 struct timespec start, stop, finish;
   
 pthread_t parray[n];
 
 
 int pindex = 0;
 
 clock_gettime (CLOCK_REALTIME, &start);
   /*
    * Make N threads, call dummy in every threads.
    */
	int ret;
	int i;
    for(i=0; i < n; i++){
		pthread_t pid;
		ret = pthread_create(&pid, NULL, dummy, NULL);
 
		if (ret == 0){//If thread was created succesfully
			parray[pindex++]=pid;
		}
		else{ //If thread creation failed.
			printf("FAILURE\n");
			exit(EXIT_FAILURE);
		}
	}
 
	clock_gettime (CLOCK_REALTIME, &stop);
	
	for(i=0;i<n;i++){
		void *result;
 
		if(pthread_join(parray[i], (void *)&result)){
 
		printf("Joined with thread %u", (unsigned)parray[i]);
 
    }
 
  }

  clock_gettime (CLOCK_REALTIME, &finish);
 
  printf ("%d proc: thread create=%5.0f (mics) wait=%5.0f (mics) sum=%5.0f (mics)\n",
   n, xelapsed (stop, start),
   xelapsed (finish, stop), xelapsed (finish, start));
  printf ("per proc: thread create=%5.0f (mics) wait=%5.0f (mics) sum=%5.0f (mics)\n",
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
