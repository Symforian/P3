#include<iostream>
#include<cstdlib>
#include "myThread.h"
  //all threads list
  myThread All_myThreads [MTHREADS_NUM];

  //Pointer to current thread running
int TEXT = 1;
int cur_myThread_ptr = -1;
 ucontext_t cleanerThread;
 myThread mainThread;
void isSomeoneWaitingFor(int Me);
void myThread_function();
int findFirstFree();
void CleanThread();
void Init_myThreads()
{

	myThread newThread;
	getcontext(&newThread.context);
	newThread.stack = malloc(MY_THREAD_STACK_SIZE);
	newThread.context.uc_link = 0;
	newThread.context.uc_stack.ss_sp = newThread.stack;
	newThread.context.uc_stack.ss_size= MY_THREAD_STACK_SIZE;
	newThread.context.uc_stack.ss_flags = 0;
	newThread.id = MAIN_THREAD;
	newThread.waitingFor = NOT_FOUND;

	mainThread = newThread;
	ucontext_t newContext;
	getcontext(&newContext);
	newContext.uc_link = 0;
	newContext.uc_stack.ss_sp =  malloc(MY_THREAD_STACK_SIZE);
	newContext.uc_stack.ss_size= MY_THREAD_STACK_SIZE;
	newContext.uc_stack.ss_flags = 0;
	makecontext(&newContext,CleanThread,0);

	cleanerThread = newContext;

	for(int i = 0; i < MTHREADS_NUM ; i ++)
		{
			All_myThreads[i].id = i;
			All_myThreads[i].isActive = false;
			All_myThreads[i].waitingFor = NOT_FOUND;
		}

}
void schedule()
{
if(TEXT)	
printf("\nSCHEDULE as %d,first empty place is %d\n",cur_myThread_ptr,findFirstFree());
	if(true)//CHANGE THIS when some kind of timer will be done.
	{
		bool swapped = false;

		if(cur_myThread_ptr == MAIN_THREAD) //We were currently doing main
		{
if(TEXT)	
printf("-MAIN\n");
				
			for(int i = 0; i < MTHREADS_NUM ; i ++)
			{	


				if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND)
					{
						if(TEXT)
						printf("---PRZEŁĄCZANIE NA %d \n\n",i);
						cur_myThread_ptr=i;
						swapped = true;
						//mainThread = currentThread;
						//currentThread = All_myThreads[i].context;

						if(swapcontext(&mainThread.context,&All_myThreads[i].context))
							printf("error");
//printf("text1");
						
					}
				
			}
		}
		else
		{
if(TEXT)	
printf("-NOT MAIN\n");
			if(!swapped)//checking myThreads current+1 to last one if ready 
			{
if(TEXT)	
printf("--current to last\n");
				for(int i = cur_myThread_ptr+1; i < MTHREADS_NUM ; i ++)
				{	if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND)
						{
						if(TEXT)
						printf("---PRZEŁĄCZANIE NA %d \n\n",i);
							swapped = true;
							//isSomeoneWaitingFor(cur_myThread_ptr);
							cur_myThread_ptr=i;
							swapcontext(&All_myThreads[cur_myThread_ptr].context,&All_myThreads[i].context);
						}
				}
			}
			if(!swapped)//swapping with main
			{
if(TEXT)	
printf("--zmiana z mainem\n");
				if(mainThread.waitingFor == MTHREADS_NUM) //main is waiting for them all
				{
if(TEXT)	
printf("--main czeka na wszystkich\n");
					if(WaitForAll_myThreads()==DONE_GOOD) //done waiting
					{
						if(TEXT)
						printf("---PRZEŁĄCZANIE NA MAIN \n\n");
						swapped = true;
					int old = cur_myThread_ptr;
					cur_myThread_ptr=MAIN_THREAD;
					swapcontext(&All_myThreads[old].context,&mainThread.context);
					}
				}
				else
					if(mainThread.waitingFor == NOT_FOUND)
				{
					if(TEXT)	
					printf("---PRZEŁĄCZANIE NA MAIN \n\n");
					swapped = true;
					int old = cur_myThread_ptr;
					cur_myThread_ptr=MAIN_THREAD;
					swapcontext(&All_myThreads[old].context,&mainThread.context);
				}
					
			}
			if(!swapped)//checking myThreads first (0) to current
			{
if(TEXT)	
printf("--od 0 do obecnego\n");
				for(int i = 0; i < cur_myThread_ptr ; i ++)
				{	if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND)
						{
						if(TEXT)
						printf("---PRZEŁĄCZANIE NA %d \n\n",i);
							swapped = true;
							//isSomeoneWaitingFor(cur_myThread_ptr);
							int old = cur_myThread_ptr;
							cur_myThread_ptr=i;
							swapcontext(&All_myThreads[old].context,&All_myThreads[i].context);
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
	myThread newThread;
	getcontext(&newThread.context);
	newThread.context.uc_link = &cleanerThread;
	newThread.context.uc_stack.ss_sp = malloc(MY_THREAD_STACK_SIZE);
	newThread.stack = newThread.context.uc_stack.ss_sp;
	newThread.context.uc_stack.ss_size= MY_THREAD_STACK_SIZE;
	newThread.context.uc_stack.ss_flags = 0;
	if(newThread.context.uc_stack.ss_sp==0)
		return DONE_WRONG;

	newThread.isActive = true;
	newThread.waitingFor = -1;
	newThread.id = place;
	All_myThreads[place]=newThread;
	makecontext(&All_myThreads[place].context,function,0);


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
void CleanThread()
{
if(TEXT)
printf("\nCLEANER\n");
	isSomeoneWaitingFor(cur_myThread_ptr);
if(TEXT)
printf("%d \n",cur_myThread_ptr);
	All_myThreads[cur_myThread_ptr].isActive = false;
	free(All_myThreads[cur_myThread_ptr].stack);

	schedule();
}

 int WaitForAll_myThreads() //check if all have finished <<<<< need to be fixed
{
	mainThread.waitingFor = MTHREADS_NUM; // waiting for all
	if(findFirstFree() == NOT_FOUND)      // no active threads
		{
			mainThread.waitingFor = NOT_FOUND;
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

void isSomeoneWaitingFor(int Me)
{
	for(int i = 0; i < MTHREADS_NUM ; i ++)
	{
		if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == Me)
			All_myThreads[i].waitingFor = NOT_FOUND;
	}
	if(mainThread.waitingFor == Me)
	{
		mainThread.waitingFor= NOT_FOUND;
	}
}
//example 
void myThread_function()
{
	for(int j = 0; j < 1; j ++)
		{
		printf("----------------%d Hello, myThread 1 there!\n",j);

		schedule();
		}
	if(TEXT)
	printf("WATEK POTOMNY 1 KONCZY PRACE");
	return;
}
void myThread_function2()
{
	for(int j = 0; j < 40; j ++)
		{
		printf("----------------%d Hello, myThread 2 there!\n",j);

		schedule();
		}
	if(TEXT)
	printf("WATEK POTOMNY 2 KONCZY PRACE");
	return;
}
int main(void)
{
	printf("Main thread\n");
	Init_myThreads();
	printf("New my thread is born:\n");
	if(Create_myThread(&myThread_function))
	printf("Error");
	if(Create_myThread(&myThread_function2))
	printf("Error");

	for(int j = 0; j < 10; j ++)
	{
	printf("(----------------%d Main thread\n\n",j);

	schedule();
	}
	
	//while(WaitForAll_myThreads()==DONE_WRONG)
	//schedule();

	return 0;
}
