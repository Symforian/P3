#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>

////STRUCTS
  //myThread struct
typedef struct{
	int place;		//place in table 
	int id;			//id for user to know which thread is which
	ucontext_t context;	
	bool isActive;		//true if there is thread, false if it's place for thread
	void* stack;
	int waitingFor;		//id of thread to wait for, when it's done this can continue
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
extern void Init_myThreads();
  // switching context between myThreads
extern void schedule();

  //Creating new myThread
extern int Create_myThread(void (*function)(void), int id);

  //Waiting for myThread to finish
extern int Join_myThread(int idToJoin);


  //Join on all remaining myThreads
extern int WaitForAll_myThreads();


/* TO DO:
// "Pushing" myThread onto semaphore
int Wait_myThread();
// "Popping" myThread of semaphore
int Signal_myThread();


*/
