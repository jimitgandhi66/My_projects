#include<stdio.h>
#include<semaphore.h>
#include<pthread.h>

#define MAX_CUSTOMER 50              // Max no of customer 
#define MAX_CUSTOMER_ALLOWED 10      // no. of customer allowed at a time
#define MAX_WORKER 3                 // no. of workers.

#define TASK_BUY_STAMP 0             
#define TASK_MAIL_LETTER 1
#define TASK_MAIL_PACKAGE 2

#define TIME_BUY_STAMP 1000         // millisecond time to buy stamp
#define TIME_MAIL_LETTER 1500       // millisecond time to mail letter
#define TIME_MAIL_PACKAGE 2000      // millisecond time to mail package

sem_t sem_max_cap;                    //  semaphore limiting customer number to 10
sem_t customer_mutex;                 //  for the critical section in customer
sem_t customer_finished[MAX_CUSTOMER]; // indicates that customer task has been finished
sem_t customer_exit[MAX_CUSTOMER];     // indicates that customer is leaving
sem_t worker_start_work;               // indicates teh worker to start work
sem_t worker_mutex;                    // critical section of the worker
sem_t scale_mutex;                     // for the use of the scale

int Customer_Queue[MAX_CUSTOMER];      // Customer Queue
int Customer_Task[MAX_CUSTOMER];       // Randomly assigned task to the customer
int front = -1;                        // front and rear value for the queue
int rear = -1;

/**
/*
/* Function for the Enqueue the customer inside the queue.
/* 
/* Argument: customer_id - customer id which you want to enqueue.
/*
**/
void EnQueue(int customer_id)
{
	rear++;
	if(rear == MAX_CUSTOMER)
	{
		rear = 0;
	}
	if(rear == front)
	{
		printf("Error in Queue adding\n");
	}
	Customer_Queue[rear] = customer_id;
}

/**
/*
/* Function for the Dequeue the customer inside the queue.
/* 
/* Returns: customer_id - customer id which you want to dequeue.
/*
**/
int DeQueue()
{
	if(front == MAX_CUSTOMER)
		front = 0;
	if(rear == front)
	{
		printf("Error in Queue fetch\n");
	}
	
	int ret = Customer_Queue[++front];
	return ret;
}

/**
/* Customer thread.
/* 
/*
*/
void* customer(void* args)
{
	int id;
	
	sem_wait(&customer_mutex);
	id = (int )args;
	EnQueue(id);
	srand(id);
	Customer_Task[id] = rand()%3;
	printf("Customer Created %d\n",id);
	sem_post(&customer_mutex);
	
	sem_wait(&sem_max_cap);
	printf("Customer Entered into post office %d\n",id);
	
	sem_post(&worker_start_work);
	
	sem_wait(&customer_finished[id]);
	
	printf("Customer %d finished\n",id);
	
	printf("Customer %d exiting\n",id);
	
	sem_post(&customer_exit[id]);
	sem_post(&sem_max_cap);
	
	pthread_exit(NULL);
}

/**
/* Worker thread.
/* 
/*
*/
void* worker(void* args)
{
	int customer_id;
	int task_id;
	int worker_id;
	
	sem_wait(&worker_mutex);
	worker_id = (int)args;
	sem_post(&worker_mutex);
	
	while(1)
	{
		char * task_string[3] = {"Buy Stamp","Mail Letter","Mail Package"};
		
		sem_wait(&worker_start_work);
		
		sem_wait(&worker_mutex);
		customer_id = DeQueue();
		task_id = Customer_Task[customer_id];
		printf("Worker %d started work with Customer %d\n",worker_id,customer_id);
		sem_post(&worker_mutex);
		
		printf("Customer %d has asked %s to Worker %d\n",customer_id,task_string[task_id],worker_id);
		switch(task_id)
		{
			case TASK_BUY_STAMP : usleep(1000 * TIME_BUY_STAMP );break;
			case TASK_MAIL_LETTER : usleep(1000 * TIME_MAIL_LETTER);break;
			case TASK_MAIL_PACKAGE :
									sem_wait(&scale_mutex);
									printf("Worker %d is using scale\n",worker_id);
									usleep(1000 * TIME_MAIL_PACKAGE);
									sem_post(&scale_mutex);
									break;
		}
		printf("Worker %d completed work\n",worker_id);
		sem_post(&customer_finished[customer_id]);
	}
}

int main()
{
	pthread_t customers[MAX_CUSTOMER];
	pthread_t workers[MAX_WORKER];
	
	int i;

	sem_init(&sem_max_cap,0,MAX_CUSTOMER_ALLOWED);
	sem_init(&customer_mutex,0,1);
	
	for(i=0;i<MAX_CUSTOMER;i++)
	{
		sem_init(&customer_finished[i],0,0);
	}

	for(i=0;i<MAX_CUSTOMER;i++)
	{
		sem_init(&customer_exit[i],0,0);
	}
	
	sem_init(&worker_start_work,0,0);
	sem_init(&worker_mutex,0,1);
	sem_init(&scale_mutex,0,1);
	
	for(i=0;i<MAX_CUSTOMER;i++)
	{
		int cust = i;
		pthread_create(&customers[i],NULL,customer,i);
	}
	
	for(i=0;i<MAX_WORKER;i++)
	{
		int work = i;
		pthread_create(&workers[i],NULL,worker,i);
	}

	for(i=0;i<MAX_CUSTOMER;i++)
	{
		sem_wait(&customer_exit[i]);	
	}
	
	for(i=0;i<MAX_CUSTOMER;i++)
	{
		pthread_join(customers[i],NULL);
		printf("Customer joined %d\n",i);
	}
	printf("Done");
}
