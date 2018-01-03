#include<ucontext.h>

//myThread struct
typedef struct{
	ucontext_t context;
	void* stack;
} myThread;

//rdy queue

// switching context between myThreads
void schedule();

//Creating new myThread
int Create_myThread(/*Arg*/);

//Waiting for myThread to finish
int Join_myThread();

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
//Join on all remaining myThreads
int WaitForAll_myThreads();

*/
