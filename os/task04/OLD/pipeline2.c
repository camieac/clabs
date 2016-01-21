#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#define N_THREADS 3
#define BUFFER_SIZE 200
#define N_DATA 100000
#define WORKLOAD1 100000
#define WORKLOAD2 100000
#define WORKLOAD3 100000
#define OUTPUT

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
	volatile int *elems;
};

buffer_t *createBuffer( int size)
{
	buffer_t *buf;

	buf = (buffer_t *)malloc( sizeof(buffer_t));
	buf->head = 0;
	buf->tail = 0;
	buf->size = size+1;
	buf->elems = (int *)malloc( (size+1)*sizeof(int));

	return( buf);
}

int pop( buffer_t* buf, int *data)
{
	int res;

	if(buf->head == buf->tail) {
	  res = 0;  
	} else {
	*data = buf->elems[buf->head];
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
	buf->elems[buf->tail] = data;
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

int workUnit( int data)
{
	if( data < 0)
	data++;

	return( data);
}

int process( int tid, int data, int  workload)
{
	int i;

	#ifdef OUTPUT
	printf( "[%d] processing item %d!\n", tid, data);
	#endif

	for( i=0; i<workload; i++){
		data = workUnit( data);
	}

	#ifdef OUTPUT
	printf( "[%d] item %d done!\n", tid, data);
	#endif

	return( data);
}

void * pipeline( void *arg)
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

	//each of the three pipeline stages should run some dummy workload
	data = in->elems[12];
	printf("data is %d\n",data);
	suc = process(tid, data, workload);
	printf("suc is %d\n",suc);
}

/*******************************************************************************
 ** 
 ** main
 ** 
 ******************************************************************************/

int main()
{
	printf("in the main method\n");
	int i, suc;
	int data;

	threadArgs_t args[N_THREADS];
	pthread_t threads[N_THREADS];
	buffer_t *in, *inter1, *inter2, *out;

	in = createBuffer( N_DATA+1);
	//process 1
	inter1 = createBuffer( BUFFER_SIZE);
	//process 2
	inter2 = createBuffer( BUFFER_SIZE);
	//process 3
	out = createBuffer( N_DATA+1);

	/**
	*
	* First, we start our threads:
	*/

	//each pipeline should be executed by an independent thread
	int status1, status2, status3;
	
	//first thread
	args[0].tid = 0;
	args[0].in_buf = in;
	args[0].out_buf = inter1;
	args[0].workload = WORKLOAD1;
	status1 = pthread_create(&threads[0], NULL, pipeline, &args[0]);	
	//if the returned value is 0 then the thread creation has been successful, otherwise an error has occured
	if(status1 != 0){
		printf("Problem with creation of thread 1.\n");
		exit(-1);
	}

	//second thread
	args[1].tid = 1;
	args[1].in_buf = inter1;
	args[1].out_buf = inter2;
	args[1].workload = WORKLOAD2;
	status2 = pthread_create(&threads[1], NULL, pipeline, &args[1]);	
	//if the returned value is 0 then the thread creation has been successful, otherwise an error has occured
	if(status2 != 0){
		printf("Problem with creation of thread 2.\n");
		exit(-1);
	}

	//third thread
	args[2].tid = 2;
	args[2].in_buf = inter2;
	args[2].out_buf = out;
	args[2].workload = WORKLOAD3;
	status3 = pthread_create(&threads[2], NULL, pipeline, &args[2]);	
	if(status3 != 0){
	//if the returned value is 0 then the thread creation has been successful, otherwise an error has occured
		printf("Problem with creation of thread 3.\n");
		exit(-1);
	}
		
	/**
	* Then, we fill the input buffer:
	*/

	// Your input please!
	//fill the input buffer with "data", comes from process?

	int j;
	printf("buffer size: %d\n",in->size);
	for(j=0; j < (in->size); j++){
		push(in,j);
		//printf("%d\n",in->elems[12]);
	}

	/**
	* Finally, we observe the output in the buffer "out":
	*/

	// Your input please!

	return(0);
}

  
