#include<iostream>
#include<cstdlib>
#include "myThread.h"
#define MY_THREAD_STACK_SIZE 1024 * 32 
//rdy queue
static myThread All_myThreads [MTHREADS_NUM];

ucontext_t currentThread, mainThread;

void Init_myThreads()
{
	
}
void schedule()
{
	//check time ?? struct do czasu
	//switch context 
		/* getcontext
		 * swapcontext
		 */
}

int Create_myThread(void (*function)(void) )
{
	void* newStack;
	newStack = malloc(MY_THREAD_STACK_SIZE);
	return 0;
}
int Join_myThread()
{
	return 0;
}
