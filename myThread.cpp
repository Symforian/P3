#include<iostream>
#include<cstdlib>
#include "myThread.h"


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
	ucontext_t new_myThread;
	void* newStack;
	newStack = malloc(MY_THREAD_STACK_SIZE);
	new_myThread.uc_link = 0;
	new_myThread.ss_sp = newStack;
	new_myThread.ss_size= MY_THREAD_STACK_SIZE;
	if(new_myThread.ss_sp==0)
	return 1;
	else 
	makecontext(&new_myThread,&function);
	return 0;
}
int Join_myThread()
{
	return 0;
}
