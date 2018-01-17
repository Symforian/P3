#include<iostream>
#include<cstdlib>
#include "myThread.h"


int main(void)
{

}
void Init_myThreads()
{
	getcontext(&mainThread);
	getcontext(&currentThread);
	for(int i = 0; i < MTHREADS_NUM ; i ++)
		{
			All_myThreads[i].id = i;
			All_myThreads[i].isActive = false;
			All_myThreads[i].waitingFor = NOT_FOUND;
		}
}
void schedule()
{
	if(true)//CHANGE THIS when some kind of timer will be done.
	{
		bool swapped = false;
		if(cur_myThread_ptr == MAIN_THREAD) //We were currently doing main
		{
			for(int i = 0; i < MTHREADS_NUM ; i ++)
			{	if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND)
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
				for(int i = cur_myThread_ptr+1; i < MTHREADS_NUM ; i ++)
				{	if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND)
						{
							swapped = true;
							isSomeoneWaitingFor(cur_myThread_ptr);
							set_myThread_ptr(i);
							swapcontext(&currentThread,&All_myThreads[i].context);
						}
				}
			}
			if(!swapped)//swapping with main
			{
				if(mainWaitingFor == MTHREADS_NUM) 
				{
					if(WaitForAll_myThreads()==DONE_GOOD) //done waiting
					{
						swapped = true;
						set_myThread_ptr(MAIN_THREAD);
						swapcontext(&currentThread,&mainThread);
					}
				}
				else
					if(mainWaitingFor == NOT_FOUND)
				{
					swapped = true;
					set_myThread_ptr(MAIN_THREAD);
					swapcontext(&currentThread,&mainThread);
				}
					
			}
			if(!swapped)//checking myThreads first (0) to current
			{
				for(int i = 0; i < cur_myThread_ptr ; i ++)
				{	if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND)
						{
							swapped = true;
							isSomeoneWaitingFor(cur_myThread_ptr);
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
	if(place == NOT_FOUND)
		return DONE_WRONG;
	getcontext(&All_myThreads[place].context);
	All_myThreads[place].stack = malloc(MY_THREAD_STACK_SIZE);
	All_myThreads[place].context.uc_link = 0;
	All_myThreads[place].context.uc_stack.ss_sp = All_myThreads[place].stack;
	All_myThreads[place].context.uc_stack.ss_size= MY_THREAD_STACK_SIZE;
	if(All_myThreads[place].context.uc_stack.ss_sp==0)
		return DONE_WRONG;

	makecontext(&All_myThreads[place].context,(void (*) (void))&runOn_myThread,1,&function);
	return DONE_GOOD;
}
int Join_myThread(myThread T)
{
	//int indexT = findIndexOfThread(T);
	if(T.id >= 0 && T.id <= MTHREADS_NUM-1)
		{
			All_myThreads[cur_myThread_ptr].waitingFor = T.id;
			return DONE_GOOD;
		}
	
	return DONE_WRONG;
}
void runOn_myThread(void (*function) (void))
{
	All_myThreads[cur_myThread_ptr].isActive = true;
	function();
	All_myThreads[cur_myThread_ptr].isActive = false;
	free(All_myThreads[cur_myThread_ptr].stack);
}
 int WaitForAll_myThreads() //check if all have finished
{
	mainWaitingFor = MTHREADS_NUM; // waiting for all
	if(findFirstFree() == NOT_FOUND)      // no active threads
		{
			mainWaitingFor = NOT_FOUND;
			return DONE_GOOD;
		}
	else
		return DONE_WRONG;
}
int findFirstFree()
{
	for(int i = 0; i < MTHREADS_NUM ; i ++)
	{
		if(All_myThreads[i].isActive == false)
			return i;
	}
	return NOT_FOUND;
}
/*int findIndexOfThread(myThread T)
{
	for(int i = 0; i < MTHREADS_NUM ; i ++)
	{
		if(All_myThreads[i].ID == T.ID)
			return i;
	}
	return NOT_FOUND;
}*/
static void set_myThread_ptr(int value)
{
	if(value >= 0 && value <= MTHREADS_NUM-1)
		cur_myThread_ptr = value;
	else
		cur_myThread_ptr = MAIN_THREAD;
}
void isSomeoneWaitingFor(int Me)
{
	for(int i = 0; i < MTHREADS_NUM ; i ++)
	{
		if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == Me)
			All_myThreads[i].waitingFor = NOT_FOUND;
	}
	if(mainWaitingFor == Me)
	{
		mainWaitingFor= NOT_FOUND;
	}
}
