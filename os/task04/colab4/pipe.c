#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define BUFFER_SIZE 200
#define N_DATA 999
#define WORKLOAD1 1
#define WORKLOAD2 1
#define WORKLOAD3 1
#define OUTPUT
struct timespec startTime, finishTime;

/*******************************************************************************
 **  
 **-------------------------------PART 4--------------------------------------- 
 **
 ** Here, the buffer implementation:
 **  
 ******************************************************************************/

typedef struct buffer buffer_t;
typedef struct dataItem dataItem_t;

struct dataItem {
	int data;
	struct timespec start;
};

struct buffer {
	volatile int head;
	volatile int tail;
	int size;
	volatile dataItem_t *elems;
	pthread_mutex_t b_mutex;
};

buffer_t *createBuffer(int size){
	buffer_t *buf;

	buf = (buffer_t *)malloc(sizeof(buffer_t));
	buf->head = 0;
	buf->tail = 0;
	buf->size = size+1;
	buf->elems = (dataItem_t *)malloc( (size+1)*sizeof(dataItem_t));

	return( buf);
}

int pop(buffer_t* buf, dataItem_t *data){
	int res;

	if(buf->head == buf->tail) {
		res = 0;  
	} else {	
		//pthread_mutex_lock(&buf->b_mutex); //Lock the mutex	
		*data = buf->elems[buf->head];		
		buf->head = (buf->head+1) % buf->size;
		//pthread_mutex_unlock(&buf->b_mutex); //Unlock the mutex
		res = 1;
	}

	return(res);
}


int push(buffer_t* buf, dataItem_t data){
	int nextTail;
	int res;
	
	nextTail = (buf->tail + 1) % buf->size;
	if(nextTail != buf->head){ 
		//pthread_mutex_lock(&buf->b_mutex); //Lock the mutex
		buf->elems[buf->tail] = data;
		buf->tail = nextTail;
		//pthread_mutex_unlock(&buf->b_mutex); //Unlock the mutex
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

dataItem_t workUnit(dataItem_t dataI){	
	dataI.data++;	
	return(dataI);
}

dataItem_t process(dataItem_t data, int  workload){
	int i;

	for(i=0; i<workload; i++){
		data = workUnit(data);
	}
	
	return(data);
}

void * pipeline(void *arg){
	int item, tid;
	dataItem_t data;	
	int workload;	
	buffer_t *in;
	buffer_t *out;	
	
	tid = ((threadArgs_t *)arg)->tid;
	in = ((threadArgs_t *)arg)->in_buf;
	out = ((threadArgs_t *)arg)->out_buf;	
	workload = ((threadArgs_t *)arg)->workload;


	if(tid == 0){
		clock_gettime (CLOCK_REALTIME, &startTime);
		int i;
		for (i = 0; i < N_DATA; i++){
			dataItem_t * dataI = malloc(sizeof(dataItem_t));
			dataI->data = i;
			clock_gettime (CLOCK_REALTIME, &dataI->start);
			if(push(out, *dataI) == 1){ //Check if push was successful
				//printf("Pushed: %i\n", i);
			}
		}	
	}else{
		for(item = 0; item < N_DATA/3; item++){									
				while(pop(in, &data) == 0);				//Pop an element from the in buffer			
				data = process(data, workload*3); 		//Process the data (3 times the processing)			
				while(push(out, data) == 0);			//Push the data to the out buffer	
		}
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

double xelapsed (struct timespec a, struct timespec b)
{
   return (a.tv_sec - b.tv_sec) * 1000000.0
          + (a.tv_nsec - b.tv_nsec) / 1000.0;
}
  
/*******************************************************************************
 ** 
 ** main
 ** 
 ******************************************************************************/

int main(){
	
	int i;	
	
	pthread_t threads[4];
	buffer_t *in, *out;

	in = createBuffer(N_DATA+1);	
	out = createBuffer(N_DATA+1);
	
	//Thread IN - In
	pthread_create(&threads[0], NULL, pipeline, createArgs(0,NULL,in,0));
	
	
	//Thread A - In -> Out
	pthread_create(&threads[1], NULL, pipeline, createArgs(1,in,out,WORKLOAD1));
	
	
	
	//Thread B - In -> Out
	pthread_create(&threads[2], NULL, pipeline, createArgs(2,in,out,WORKLOAD2));
	
	
	
	//Thread C - In -> Out
	pthread_create(&threads[3], NULL, pipeline, createArgs(3,in,out,WORKLOAD3));
	
	
	
		
	/**
	 * Finally, we observe the output in the buffer "out":
	 */	
	double totalLatency, latency, minLatency, maxLatency, avgLatency, throughput, totalTime;
	totalTime = 0;
	totalLatency = 0;
	latency = 0;
	minLatency = 10000000000000;
	maxLatency = 0;
	avgLatency = 0;
	
	dataItem_t dataI;
	struct timespec finish;
	for(i = 0; i < N_DATA; i++){		
		while(pop(out, &dataI) == 0);
		clock_gettime (CLOCK_REALTIME, &finish);
		latency = xelapsed(finish, dataI.start);
		printf("Latency: %f mics\n", latency);
		totalLatency += latency;
		if(minLatency > latency) minLatency = latency;
		if(maxLatency < latency) maxLatency = latency;
	}
	clock_gettime (CLOCK_REALTIME, &finishTime);
	totalTime = xelapsed(finishTime, startTime);
	avgLatency = totalLatency / N_DATA;
	throughput = N_DATA / (1000000/ totalTime);
	printf("Average Throughput: %f Items/s\n", throughput);
	printf("Average Latency: %f mics\n", avgLatency);
	printf("Min Latency: %f mics\n", minLatency);
	printf("Max Latency: %f mics\n", maxLatency);
	
}


