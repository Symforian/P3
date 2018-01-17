#include<iostream>
#include<cstdlib>
#include "myThread.h"


int main(void)
{

}
void Init_myThreads()
{
	for(int i = 0; i < MTHREADS_NUM-1 ; i ++)
		All_myThreads[i].isActive = false;
}
void schedule()
{
	if(true)//CHANGE THIS when some kind of timer will be done.
	{
		bool swapped = false;
		if(cur_myThread_ptr == -1) //We were currently doing main
		{
			for(int i = 0; i < MTHREADS_NUM-1 ; i ++)
			{	if(All_myThreads[i].isActive == true)
					{
						swapped = true;
						set_myThread_ptr(i);
						swapcontext(&currentThread,&All_myThreads[i].context);
					}
			}
		}
		else
		{
			if(!swapped)//checking myThreads current+1 to last one if ready 
			{
				for(int i = cur_myThread_ptr+1; i < MTHREADS_NUM-1 ; i ++)
				{	if(All_myThreads[i].isActive == true)
						{
							swapped = true;
							set_myThread_ptr(i);
							swapcontext(&currentThread,&All_myThreads[i].context);
						}
				}
			}
			if(!swapped)//swapping with main
			{
				swapped = true;
				set_myThread_ptr(-1);
				swapcontext(&currentThread,&mainThread);
					
			}
			if(!swapped)//checking myThreads first (0) to current-1
			{
				for(int i = 0; i < cur_myThread_ptr-1 ; i ++)
				{	if(All_myThreads[i].isActive == true)
						{
							swapped = true;
							set_myThread_ptr(i);
							swapcontext(&currentThread,&All_myThreads[i].context);
						}
				}
			}
		}
	}
}

int Create_myThread(void (*function)(void) )
{
	int place = findFirstFree();
	if(place == -1)
		return 1;
	All_myThreads[place].stack = malloc(MY_THREAD_STACK_SIZE);
	All_myThreads[place].context.uc_link = 0;
	All_myThreads[place].context.uc_stack.ss_sp = All_myThreads[place].stack;
	All_myThreads[place].context.uc_stack.ss_size= MY_THREAD_STACK_SIZE;
	if(All_myThreads[place].context.uc_stack.ss_sp==0)
	return 1;

	makecontext(&All_myThreads[place].context,(void (*) (void))&runOn_myThread,1,&function);
	return 0;
}
int Join_myThread(myThread T)//tbd
{
	
	
	return 0;
}
void runOn_myThread(void (*function) (void))
{
	All_myThreads[cur_myThread_ptr].isActive = true;
	function();
	All_myThreads[cur_myThread_ptr].isActive = false;
	free(All_myThreads[cur_myThread_ptr].stack);
}
 int WaitForAll_myThreads() //check if all have finished, need fixing
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
