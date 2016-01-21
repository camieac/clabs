#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


#define BUFFER_SIZE 200
#define N_DATA 10
#define WORKLOAD1 10000
#define WORKLOAD2 10000
#define WORKLOAD3 10000
#define OUTPUT
pthread_mutex_t a_mutex;
pthread_cond_t a_push;
pthread_mutex_t b_mutex;
pthread_cond_t b_push;

/*******************************************************************************
 **  
 **-------------------------------PART 1--------------------------------------- 
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

buffer_t *createBuffer(int size){
	buffer_t *buf;

	buf = (buffer_t *)malloc(sizeof(buffer_t));
	buf->head = 0;
	buf->tail = 0;
	buf->size = size+1;
	buf->elems = (int *)malloc( (size+1)*sizeof(int));

	return( buf);
	}

int pop(buffer_t* buf, int *data){
	int res;

	if(buf->head == buf->tail) {
		res = 0;  
	} else {
		*data = buf->elems[buf->head];
		buf->head = (buf->head+1) % buf->size;
		res = 1;
	}

	return(res);
}


int push(buffer_t* buf, int data){
	int nextTail;
	int res;

	nextTail = (buf->tail + 1) % buf->size;
	if(nextTail != buf->head){ 
		buf->elems[buf->tail] = data;
		buf->tail = nextTail;
		res = 1;
	  } else {
		res = 0;
	}

	return(res);
}

/*******************************************************************************
 **  
 ** Now, the thread functions for the pipelining:
 **  
 ******************************************************************************/

typedef struct threadArgs threadArgs_t;

struct threadArgs{
	int tid;
	buffer_t *in_buf;
	buffer_t *out_buf;
	int workload;
};

int workUnit(int data){	
	data++;	
	return(data);
}

int process(int data, int  workload){
	int i;

	for(i=0; i<workload; i++){
		data = workUnit(data);
	}
	
	return(data);
}

void * pipeline(void *arg){
	int item;
	int data;	
	int workload;	
	buffer_t *in;
	buffer_t *out;	
	in = ((threadArgs_t *)arg)->in_buf;
	out = ((threadArgs_t *)arg)->out_buf;	
	workload = ((threadArgs_t *)arg)->workload;


	

	for(item = 0; item < N_DATA; item++){									
			while(pop(in, &data) == 0);				//Pop an element from the in buffer			
			data = process(data, workload); 		//Process the data			
			while(push(out, data) == 0);			//Push the data to the out buffer	
	}
	
	return 0;
}



threadArgs_t * createArgs(int id, buffer_t * in, buffer_t * out, int work){
	threadArgs_t * args;
	args = malloc(sizeof(threadArgs_t));
	args->tid = id;
	args->in_buf = in;
	args->out_buf = out;
	args->workload = work;
	return args;
}
  
/*******************************************************************************
 ** 
 ** main
 ** 
 ******************************************************************************/

int main(){
	
	
	int data, i;

	
	pthread_t threads[3];
	buffer_t *in, *bufA, *bufB, *out;

	in = createBuffer(N_DATA+1);
	bufA = createBuffer(BUFFER_SIZE);
	bufB = createBuffer(BUFFER_SIZE);
	out = createBuffer(N_DATA+1);
	
	//Thread A - In -> bufA	
	pthread_create(&threads[0], NULL, pipeline, createArgs(0,in,bufA,WORKLOAD1));
	
	
	
	//Thread B - bufA -> bufB
	pthread_create(&threads[1], NULL, pipeline, createArgs(1,bufA,bufB,WORKLOAD2));
	
	
	
	//Thread C - bufB -> out
	pthread_create(&threads[2], NULL, pipeline, createArgs(2,bufB,out,WORKLOAD3));
	
	
	
	
	
	
	
	/**
	 * Then, we fill the input buffer:
	 */
	for (i = 0; i < N_DATA; i++){
		if(push(in, i) == 1){ //Check if push was successful
			printf("Pushed: %i\n", in->elems[i]);
		}
	}
		
	/**
	 * Finally, we observe the output in the buffer "out":
	 */
	
	
	for(i = 0; i < N_DATA; i++){
		while(pop(out, &data) == 0);
		printf("Popped %d\n", data);
	}
	
}


