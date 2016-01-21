  #include <stdlib.h>
  #include <stdio.h>
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/wait.h>
  #include <sys/time.h>
  #include <time.h>
  #include <pthread.h>

typedef struct Queue
{
	int capacity;
	int size;
	int front;
	int rear;
	int *elements;
}Queue;

typedef struct Argument
{
	Queue* queue;
	int n;
	int * start;
}Argument;

Queue * create_queue(int maxElements)
{
        /* Create a Queue */
        Queue *Q;
        Q = (Queue *)malloc(sizeof(Queue));
        /* Initialise its properties */
        Q->elements = (int *)malloc(sizeof(int)*maxElements);
        Q->size = 0;
        Q->capacity = maxElements;
        Q->front = 0;
        Q->rear = -1;
        /* Return the pointer */
        return Q;
}
Argument * create_argument(Queue *q,int n, int * start)
{
        Argument *arg;
        arg = (Argument *)malloc(sizeof(Argument));
        
        arg->queue = q;//(Queue *)malloc(sizeof(Queue)*q);
        arg->n = n;
        arg->start = start;
        return arg;
}
void dequeue(Queue *Q){
	/* If Queue size is zero then it is empty. So we cannot pop */
	if(Q->size==0){
		printf("Queue is Empty\n");
		return;
	}
	/* Removing an element is equivalent to incrementing index of front by one */
	else{
		Q->size--;
		printf("Dequed %i\n", Q->elements[Q->front]);
		Q->front++;
		/* As we fill elements in circular fashion */
		if(Q->front==Q->capacity){
			Q->front=0;
		}
	}
	return;
}
int front(Queue *Q){
	if(Q->size==0){
		printf("Queue is Empty\n");
		exit(0);
	}
	/* Return the element which is at the front*/
	return Q->elements[Q->front];
}
void enqueue(Queue *Q,int element){
	/* If the Queue is full, we cannot push an element into it as there is no space for it.*/
	if(Q->size == Q->capacity){
		printf("Queue is Full\n");
	}
	else{
		Q->size++;
		Q->rear = Q->rear + 1;
		/* As we fill the queue in circular fashion */
		if(Q->rear == Q->capacity){
			Q->rear = 0;
		}
		/* Insert the element in its rear side */ 
		Q->elements[Q->rear] = element;
		printf("Enqueued %i\n",Q->elements[Q->rear]);
	}
	return;
}


void * producer_pos (void * arg)
{
	
	int i;
	Argument *a = (Argument*) arg;
	while((int)a->start != (int)1){
		usleep(10);
	}
	for(i = 1; i <= a->n;i++){
		enqueue(a->queue,i);
	}
 return arg;
}
void * producer_neg (void * arg)
{
	int i;
	Argument *a = (Argument*) arg;
	while((int)a->start != 1){
		usleep(10);
	}
	int n = (int) a->n;
	for(i = -1; i >= -n;i--){
		enqueue(a->queue,i);
	}
 return arg;
}

void * consumer (void * arg)
{
	Argument *a = (Argument*) arg;
	while((int)a->start != 1){
		usleep(10);
	}
	int i = 0;
	int n = a->queue->size;
	for(i = 0; i < n; i++){
		dequeue(a->queue);
	}
 return arg;
}
 
pthread_t create_producer_pos(void * arg){
		pthread_t tid;
		int r = pthread_create(&tid, NULL, producer_pos, arg);
		if(r == 0) printf("pos producer TID %i created\n",tid); 
		else printf("producer creation failed");
		return tid;
}
pthread_t create_producer_neg(void * arg){
		pthread_t tid;
		int r = pthread_create(&tid, NULL, producer_neg, arg);
		if(r == 0) printf("neg producer TID %i created\n",tid); 
		else printf("producer creation failed");
		return tid;
}

pthread_t create_consumer(void * q){
		pthread_t tid;
		int r = pthread_create(&tid, NULL, consumer, (void *)q);
		if(r == 0) printf("Consumer TID %i created\n",tid); 
		else printf("consumer creation failed");
		return tid;
}
 
void create_threads (void * arg)
  {  
	pthread_t parray[4];
	parray[0] = create_producer_pos(arg);
	parray[1] = create_producer_neg(arg);
	/* The consumers don't need n, so we just give them the queue.
	 */ 

	parray[2] = create_consumer(arg);
	parray[3] = create_consumer(arg);
	
	
	sleep(1);
	Argument *a = (Argument *) arg;
	a->start = 1;
	
	int i;
	for(i=0;i<4;i++){
		void *result;
		if(pthread_join(parray[i], (void *)&result)){
			printf("Joined with thread %u", (unsigned)parray[i]);
		}
	}
  }  

 
int main()
{
	int * start = malloc(sizeof(int));
	start = 0;
	Queue *q = create_queue(20);
	Argument *arg = create_argument(q,10,start);
	create_threads((void *)arg);
	printf("Created Threads\n");	
	printf("Complete\n");
  return(0);
}

