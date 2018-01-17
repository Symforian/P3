#include<iostream>
#include<cstdlib>
#include "myThread.h"


ucontext_t currentThread, mainThread;

void Init_myThreads()
{
	for(int i = 0; i < MTHREADS_NUM-1 ; i ++)
		All_myThreads[i].isActive = false;
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
	int place = findFirstFree();
	if(place == -1)
	return 1;
	//ucontext_t new_myThread;
	void* newStack;
	newStack = malloc(MY_THREAD_STACK_SIZE);
	All_myThreads[place].uc_link = 0;
	All_myThreads[place].ss_sp = newStack;
	All_myThreads[place].ss_size= MY_THREAD_STACK_SIZE;
	if(All_myThreads[place].ss_sp==0)
	return 1;
	else 
	makecontext(&new_myThread,&function);
	return 0;
}
int Join_myThread(myThread T)
{
	
	
	return 0;
}
 int WaitForAll_myThreads()
{
	if(findFirstFree() == -1)
		return 0;
	else
		return -1;
}
int findFirstFree()
{
	for(int i = 0; i < MTHREADS_NUM-1 ; i ++)
	{
		if(All_myThreads[i].isActive == false)
			return i;
	}
	return -1;
}
