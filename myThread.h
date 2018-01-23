#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>

////STRUCTS
  //myThread struct
typedef struct{
	int id;			
	ucontext_t context;
	bool isActive;
	void* stack;
	int waitingFor;
} myThread;
////FIELDS
  //max thread stack size
#define MY_THREAD_STACK_SIZE 1024 * 64 
  //max thread number
#define MTHREADS_NUM 10
  //Signals
#define DONE_GOOD 0
#define NOT_FOUND -1 
#define MAIN_THREAD -1
#define DONE_WRONG 1




////FUNCTIONS
void Init_myThreads();
  // switching context between myThreads
void schedule();

  //Creating new myThread
int Create_myThread(void (*function)(void) );

  //Waiting for myThread to finish
int Join_myThread(myThread T);

  //Running function on thread
void runOn_myThread(void (*function) (void));

  //Join on all remaining myThreads
int WaitForAll_myThreads();

  //finding place for new thread in thread queue
//int findFirstFree();

  //setter for pointer 
// void set_myThread_ptr(int value);

  //Makes those who waited running again
//void isSomeoneWaitingFor(int Me);

  //Finds index in table of all threads of given thread 
//int findIndexOfThread(myThread T);
/*
enum Signals
{
	OK = 0,
	OUT_OF_MEM = -1
};*/
/* TO DO:
// "Pushing" myThread onto semaphore
int Wait_myThread();
// "Popping" myThread of semaphore
int Signal_myThread();


*/
