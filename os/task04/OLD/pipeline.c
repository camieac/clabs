#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define N_THREADS 3
#define BUFFER_SIZE 200
#define N_DATA 1000
#define WORKLOAD1 100000
#define WORKLOAD2 100000
#define WORKLOAD3 100000
//#define OUTPUT

/*******************************************************************************
 **  
 ** Here, the buffer implementation:
 **  
 ******************************************************************************/

typedef struct buffer buffer_t;

struct buffer {
  volatile int head;
  volatile int tail;
  int size;
  volatile int *elements;
};

buffer_t *createBuffer( int size)
{
  buffer_t *buf;

  buf = (buffer_t *)malloc( sizeof(buffer_t));
  buf->head = 0;
  buf->tail = 0;
  buf->size = size+1;
  buf->elements = (int *)malloc( (size+1)*sizeof(int));

  return( buf);
}

int pop( buffer_t* buf, int *data)
{
  int res;

  if(buf->head == buf->tail) {
      res = 0;  
  } else {
    *data = buf->elements[buf->head];
    buf->head = (buf->head+1) % buf->size;
    res = 1;
  }

  return( res);
}

int push( buffer_t* buf, int data)
{
  int nextTail;
  int res;

  nextTail = (buf->tail + 1) % buf->size;
  if(nextTail != buf->head)   { 
    buf->elements[buf->tail] = data;
    buf->tail = nextTail;
    res = 1;
  } else {
    res = 0;
  }

  return( res);
}

/*******************************************************************************
 **  
 ** Now, the thread functions for the pipelining:
 **  
 ******************************************************************************/

typedef struct threadArgs threadArgs_t;

struct threadArgs {
  int tid;
  buffer_t *in_buf;
  buffer_t *out_buf;
  int workload;
};
/**	int workUnit(int data)
 *  Increment the integer parameter by one and return it.
 * 	Only works on positive data values.
 */ 
int workUnit(int data)
{
  if(data < 0)
    data++;

  return(data);
}

int process( int tid, int data, int  workload)
{
  int i;

#ifdef OUTPUT
    printf( "[%d] processing item %d!\n", tid, data);
#endif

  for(i=0; i<workload; i++) //
    data = workUnit(data);

#ifdef OUTPUT
    printf( "[%d] item %d done!\n", tid, data);
#endif

  return(data);
}

void * pipeline(void *arg)
{
  int data;
  int workload;
  int suc;
  buffer_t *in;
  buffer_t *out;
  int tid;

  in = ((threadArgs_t *)arg)->in_buf;
  out = ((threadArgs_t *)arg)->out_buf;
  tid = ((threadArgs_t *)arg)->tid;
  workload = ((threadArgs_t *)arg)->workload;
  #ifdef OUTPUT
	printf("Running pipeline function for tid &%d\n", tid);
  #endif
//  Here, your input is required :-)
//for(data = 0; data < workload; data++){
int i;
while(1){
	//i = 0;
	//while(out->size !=0){
	//	pop(out,out->elements[i];
		
	//}
	//for(data = 0; data < workload; data++){
	//	push(out,in->elements[data]);
	//}
	*out = *in;
	
}
	return arg; //just cause
}

int check_success (int  suc){
	if (suc == 0){
		#ifdef OUTPUT
			printf("Created thread successfully\n");
		#endif
		return 0;
	}
	if(suc == -1){
		#ifdef OUTPUT
			printf("Failed to create thread\n");
		#endif
		exit(EXIT_FAILURE);
		return -1;
	}
	return 1;
}

void print_buffer(buffer_t * buf){
	printf("buffer :[");
	int i;
  for(i = 0; i < buf->size -2;i++){
	  printf("%i",buf->elements[i]);
	  if(i != buf->size-3) printf(",");
  }
  printf("]\n");
}
/*******************************************************************************
 ** 
 ** main
 ** 
 ******************************************************************************/

int main()
{
  int i, suc;
  int data;

  threadArgs_t *args[N_THREADS];
  pthread_t threads[N_THREADS];
  buffer_t *in, *inter1, *inter2, *out;

  in = createBuffer( N_DATA+1);
  inter1 = createBuffer( BUFFER_SIZE);
  inter2 = createBuffer( BUFFER_SIZE);
  out = createBuffer( N_DATA+1);
  
  /**
   *
   * First, we start our threads:
   */
  
	//My code
	
	void* result;
	
	args[0] = malloc(sizeof(threadArgs_t));
	args[0]->tid = threads[0];
	args[0]->in_buf = in;
	args[0]->out_buf = inter1;
	args[0]->workload = WORKLOAD1;
	suc = pthread_create(&threads[0],NULL,pipeline,args[0]);
	check_success(suc);
	//pthread_join(threads[0], (void *)&result);
	
	
	args[1] = malloc(sizeof(threadArgs_t));
	args[1]->tid = threads[1];
	args[1]->in_buf = inter1;
	args[1]->out_buf = inter2;
	args[1]->workload = WORKLOAD2;
	suc = pthread_create(&threads[1],NULL,pipeline,args[1]);
	check_success(suc);
	//pthread_join(threads[1], (void *)&result);
	
	
	args[2] = malloc(sizeof(threadArgs_t));
	args[2]->tid = threads[2];
	args[2]->in_buf = inter2;
	args[2]->out_buf = out;
	args[2]->workload = WORKLOAD3;
	suc = pthread_create(&threads[2],NULL,pipeline,args[2]);
	check_success(suc);
	//pthread_join(threads[2], (void *)&result);
	
	///My code
  
  /**
   * Then, we fill the input buffer:
   */
 // My Code
  for(i = 0; i < N_DATA; i++){
	  push(in,1);
  }
  #ifdef OUTPUT
  printf("Input Buffer: ");
  print_buffer(in);
  #endif
  ///My Code
  sleep(2);

  /**
   * Finally, we observe the output in the buffer "out":
   */
   print_buffer(in);
	print_buffer(out);
	
  return(0);
}
