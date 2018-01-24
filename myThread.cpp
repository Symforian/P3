#include<iostream>
#include<cstdlib>
#include "myThread.h"
  //all threads list
  myThread All_myThreads [MTHREADS_NUM];

  //Pointer to current thread running
int cur_myThread_ptr = -1;
int mainWaitingFor = -1;
 ucontext_t currentThread;
 myThread mainThread;
void isSomeoneWaitingFor(int Me);
void set_myThread_ptr(int value);
int findFirstFree();
void Init_myThreads()
{
	//getcontext(&mainThread);
	myThread newThread;
	getcontext(&newThread.context);
	newThread.stack = malloc(MY_THREAD_STACK_SIZE);
	newThread.context.uc_link = 0;
	newThread.context.uc_stack.ss_sp = newThread.stack;
	newThread.context.uc_stack.ss_size= MY_THREAD_STACK_SIZE;
	newThread.id = MAIN_THREAD;
	newThread.waitingFor = NOT_FOUND;
	newThread.context.uc_stack.ss_flags = 0;
	mainThread = newThread;
	//currentThread = newThread.context;
	//getcontext(&mainThread);
	//getcontext(&currentThread);
	//currentThread.uc_link = 0;
	//currentThread.uc_stack.ss_sp = malloc(MY_THREAD_STACK_SIZE);
	//currentThread.uc_stack.ss_size= MY_THREAD_STACK_SIZE;
	//getcontext(&currentThread);
	for(int i = 0; i < MTHREADS_NUM ; i ++)
		{
			All_myThreads[i].id = i;
			All_myThreads[i].isActive = false;
			All_myThreads[i].waitingFor = NOT_FOUND;
		}
	//swapcontext(&mainThread.context,&currentThread);
}
void schedule()
{	//printf("hello");
//	if(true)//CHANGE THIS when some kind of timer will be done.
	//{
		bool swapped = false;
	printf("hello");
		if(cur_myThread_ptr == MAIN_THREAD) //We were currently doing main
		{
				//printf("hello");
			for(int i = 0; i < MTHREADS_NUM ; i ++)
			{	//printf("%d 111111111111111111111111111111",i);
	//printf("hello");
		//printf("Thread %d is active? %d\n",i,All_myThreads[i].isActive);
//if(i==0)
//printf("Thread %d is active? %d\n",i,All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND);
						printf("hello");
				if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND)
					{
				printf("%d w4 %d",All_myThreads[i].id,All_myThreads[i].waitingFor);
						
						//set_myThread_ptr(i);
						//swapped = true;
						//mainThread = currentThread;
						//currentThread = All_myThreads[i].context;

						if(swapcontext(&mainThread.context,&All_myThreads[i].context))
							printf("error");

						
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
							//swapcontext(&currentThread,&All_myThreads[i].context);
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
						//swapcontext(&currentThread,&mainThread.context);
					}
				}
				else
					if(mainWaitingFor == NOT_FOUND)
				{
					swapped = true;
					set_myThread_ptr(MAIN_THREAD);
					//swapcontext(&currentThread,&mainThread.context);
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
							//swapcontext(&currentThread,&All_myThreads[i].context);
						}
				}
			}
		}
	//}
}

int Create_myThread(void (*function)(void) )
{
	printf("1\n");
	int place = findFirstFree();
	printf("1,%d",place);
	if(place == NOT_FOUND)
		return DONE_WRONG;
	myThread newThread;
	//getcontext(&newThread.context);
	newThread.stack = malloc(MY_THREAD_STACK_SIZE);
	newThread.context.uc_link = &mainThread.context;
	newThread.context.uc_stack.ss_sp = newThread.stack;
	newThread.context.uc_stack.ss_size= MY_THREAD_STACK_SIZE;
	newThread.context.uc_stack.ss_flags = 0;
	if(newThread.context.uc_stack.ss_sp==0)
		return DONE_WRONG;
	printf("Thread is active? creat %d\n",All_myThreads[place].isActive);
	newThread.isActive = true;
	newThread.waitingFor = -1;
	newThread.id = place;
	All_myThreads[place]=newThread;
	makecontext(&All_myThreads[place].context,(void (*) (void))&runOn_myThread,1,&function);


	printf("Thread is activeef? creat %d\n",All_myThreads[place].isActive);
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
{printf("1b");
	All_myThreads[cur_myThread_ptr].isActive = true;
	function();
	printf("1e");
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
 void set_myThread_ptr(int value)
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
//example 
void myThread_function()
{
	for(int j = 0; j < 40; j ++)
		{
		printf("Hello, myThread there!\n");
		//schedule();
		}
}
int main(void)
{
	printf("Main thread\n");
	Init_myThreads();
	printf("New my thread is born:\n");
	if(Create_myThread(&myThread_function))
	printf("Error");
	printf("New my thread is born:\n");
	for(int j = 0; j < 1; j ++)
	{
	printf("Main thread");
	//printf("hello");
	schedule();
	}
	WaitForAll_myThreads();
	/*myThread newThread;
	getcontext(&newThread.context);
	newThread.stack = malloc(MY_THREAD_STACK_SIZE);
	newThread.context.uc_link = 0;
	newThread.context.uc_stack.ss_sp = newThread.stack;
	newThread.context.uc_stack.ss_size= MY_THREAD_STACK_SIZE;
	newThread.context.uc_stack.ss_flags = 0;	
	newThread.id = 0;
	newThread.waitingFor = 0;
	//makecontext(&newThread.context,myThread_function,0);
	myThread newThread2;
	//getcontext(&newThread2.context);
	newThread2.stack = malloc(MY_THREAD_STACK_SIZE);
	newThread2.context.uc_link = 0;
	newThread2.context.uc_stack.ss_sp = newThread2.stack;
	newThread2.context.uc_stack.ss_size= MY_THREAD_STACK_SIZE;
	newThread2.context.uc_stack.ss_flags = 0;
	newThread2.id = 0;
	newThread2.waitingFor = 0;
	makecontext(&newThread2.context,myThread_function,0);
	swapcontext(&newThread.context,&newThread2.context);*/
	return 0;
}
