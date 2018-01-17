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
	//swap with main thread???
	if(/*"Time passed"*/)
	{
		bool swapped = false;
		for(int i = cur_myThread_ptr; i < MTHREADS_NUM-1 ; i ++)
			if(All_myThreads[i].isActive == true)
				{
					swapped = true;
					swapcontext();
				}
		if(!swapped)
		for(int i = 0; i < cur_myThread_ptr-1 ; i ++)
			if(All_myThreads[i].isActive == true)
				{
					swapped = true;
					swapcontext();
				}
	}
}

int Create_myThread(void (*function)(void) )
{
	int place = findFirstFree();
	if(place == -1)
	return 1;
	//ucontext_t new_myThread;
	//void* newStack;
	All_myThreads[place].stack = malloc(MY_THREAD_STACK_SIZE);
	All_myThreads[place].uc_link = 0;
	All_myThreads[place].ss_sp = All_myThreads[place].stack;
	All_myThreads[place].ss_size= MY_THREAD_STACK_SIZE;
	if(All_myThreads[place].ss_sp==0)
	return 1;

	makecontext(&new_myThread,&function,0);
	return 0;
}
int Join_myThread(myThread T)
{
	
	
	return 0;
}
 int WaitForAll_myThreads() //check if all have finished 
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
static void set_myThread_ptr(int value)
{
	if(value >= 0 && value <= MTHREADS_NUM-1)
		cur_myThread_ptr = value;
	else
		cur_myThread_ptr = -1;
}
