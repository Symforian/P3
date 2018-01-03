#include<ucontext.h>

//myThread struct
typedef struct{
	ucontext context;
} myThread;

// switching context between myThreads
void schedule();

//Creating new myThread
int Create_myThread(/*Arg*/);

//Waiting for myThread to finish
int Join_myThread();


/* TO DO:
// "Pushing" myThread onto semaphore
int Wait_myThread();
// "Popping" myThread of semaphore
int Signal_myThread();
//Join on all remaining myThreads
int WaitForAll_myThreads();
*/
