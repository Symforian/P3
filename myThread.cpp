#include<iostream>
#include<cstdlib>
#include "myThread.h"
//rdy queue
static myThread All_myThreads [MTHREADS_NUM];


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
	return 0;
}
int Join_myThread()
{
	return 0;
}
