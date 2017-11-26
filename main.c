//include necessary headers   
#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include <time.h> 
//define some macros
#define BUFFER_SIZE 3			// SIZE OF BUFFER
#define LOOP_CYCLE 7			// CYCLES OF LOOP
typedef int buf_type;
//define handles of semaphores & mutex  
HANDLE m_M_mutex ;

HANDLE m_s_empty,m_s_full;
//define buffer
buf_type buffer[BUFFER_SIZE]={-1};
int buffer_pointer=-1;				//set to 1 because producer will add first
//PRODUCER
void *producer(void *param){
	
    buf_type randb;
   
    srand((unsigned)time(NULL));
    int count=0;
    while(TRUE){
	
	
	//create a value
	randb= rand();
	printf("\nProduce %d \n",randb);
	
	// first wait(empty) then wait(mutex)   otherwise, deadlock
	
	//test if buffer has enough space
	WaitForSingleObject(m_s_empty,INFINITE);
	//success
	//test if buffer is available
	WaitForSingleObject(m_M_mutex,INFINITE);
	
	//success
	//put data to buffer (buffer pointer ++ first)
	
	buffer_pointer++;
	buffer[buffer_pointer]=randb;
	
	printf("data was put into #%d position\n",buffer_pointer);
		
	ReleaseSemaphore(m_s_full,1,NULL);
	ReleaseMutex(m_M_mutex); 
	
	
	
	count++;

 	if (count>LOOP_CYCLE)	break;
    }
    
    
    printf("Thread producer exited\n");
    ExitThread(0);
}
//CONSUMER 
void *consumer(void *param){
    buf_type randb;
  	
    srand((unsigned)time(NULL));
    int count=0;
    while(TRUE){
	
	//consumer sleeps instead of producer because without data it cannot consume
	 Sleep(rand()%5);			//sleep so it will test the mutex and semaphore
	

        
    // first wait(full) then wait(mutex)   otherwise, deadlock
        
	//test if buffer has data
	WaitForSingleObject(m_s_full,INFINITE);
		//success
	//test if buffer is available
	WaitForSingleObject(m_M_mutex,INFINITE);
	//success
	//get data from buffer (buffer pointer-- then)
	buf_type n=buffer[buffer_pointer];
	buffer_pointer--;
	
	printf("data %d from #%d position was consumed\n",n,buffer_pointer+1);
		
	ReleaseSemaphore(m_s_empty,1,NULL);
	ReleaseMutex(m_M_mutex); 
	
	count++;

 	if (count>LOOP_CYCLE)	break;
    }
    printf("Thread consumer exited");
     ExitThread(0);
}

//mian FUNCTION
int main(int argc,char *argv[])
{
    m_M_mutex = CreateMutex(NULL,FALSE,NULL);	//MUTEX
    
    m_s_empty = CreateSemaphore(NULL,BUFFER_SIZE,BUFFER_SIZE,NULL);	//PRODUCER
    m_s_full = CreateSemaphore(NULL,0,BUFFER_SIZE,NULL);			//CONSUMER 
    
    DWORD test=1;
    DWORD id_of_consumer=0;
    DWORD id_of_producer=0;
    
    printf("Creating consumer thread and producer thread\n");
    
    HANDLE h_consumer=CreateThread(NULL,0,&consumer,&test,0,&id_of_consumer);
    HANDLE h_producer=CreateThread(NULL,0,&producer,&test,0,&id_of_producer);
    
    printf("producer thread id:%d\n",id_of_producer);
    printf("consumer thread id:%d\n",id_of_consumer);

    system("pause");

    return 0;
}
