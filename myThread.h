#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>
#define MY_THREAD_STACK_SIZE 1024 * 32 
//all threads list
static myThread All_myThreads [MTHREADS_NUM];
//myThread struct
typedef struct{
	ucontext_t context;
	bool isActive;
	void* stack;
} myThread;
//max thread number
#define MTHREADS_NUM 10
//Pointer to current thread running
static int cur_myThread_ptr = -1;

extern void Init_myThreads();
// switching context between myThreads
extern void schedule();

//Creating new myThread
extern int Create_myThread(void (*function)(void) );

//Waiting for myThread to finish
extern int Join_myThread(myThread T);

//Join on all remaining myThreads
extern int WaitForAll_myThreads();

//finding place for new thread in thread queue
extern int findFirstFree();

//setter for pointer 
static void set_myThread_ptr(int value);


enum Signals
{
	OK = 0,
	OUT_OF_MEM = -1,
}
/* TO DO:
// "Pushing" myThread onto semaphore
int Wait_myThread();
// "Popping" myThread of semaphore
int Signal_myThread();


*/
