#include<stdio.h>
#include<stdlib.h>
#include<ucontext.h>

#define MY_THREAD_STACK_SIZE 1024 * 32 
//max thread number
#define MTHREADS_NUM 10

//myThread struct
typedef struct{
	ucontext_t context;
	bool isActive;
	void* stack;
} myThread;
//all threads list
static myThread All_myThreads [MTHREADS_NUM];

//Pointer to current thread running
static int cur_myThread_ptr = -1;


ucontext_t currentThread;
static ucontext_t mainThread;

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
int findFirstFree();

//setter for pointer 
static void set_myThread_ptr(int value);


enum Signals
{
	OK = 0,
	OUT_OF_MEM = -1
};
/* TO DO:
// "Pushing" myThread onto semaphore
int Wait_myThread();
// "Popping" myThread of semaphore
int Signal_myThread();


*/
