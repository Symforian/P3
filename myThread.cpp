#include<iostream>
#include<cstdlib>
#include "myThread.h"
  //all threads list
  myThread All_myThreads [MTHREADS_NUM];
int TEXT =0;
int cur_myThread_ptr = -1;
 ucontext_t cleanerThread;
 myThread mainThread;
//Functions
void isSomeoneWaitingFor(int Me); 	//'awakes' every thread waiting for Me
int findFirstFree();			//returns first empty place to put new myThread 
int running_myThreads();		//returns amount of running myThreads (running means active)
void CleanThread();			//cleans stack of myThread and deactivates it
int canI_JoinHim(int I, int Him);	//checks if Him is not waiting for someone who is waiting for me (kind of deadlock prevention)
int placeFromId(int id);		//returns given id's place
void Init_myThreads()
{

	myThread newThread;
	getcontext(&newThread.context);
	newThread.stack = malloc(MY_THREAD_STACK_SIZE);
	newThread.context.uc_link = 0;
	newThread.context.uc_stack.ss_sp = newThread.stack;
	newThread.context.uc_stack.ss_size= MY_THREAD_STACK_SIZE;
	newThread.context.uc_stack.ss_flags = 0;
	newThread.place = MAIN_THREAD;
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
			All_myThreads[i].id = 0;
			All_myThreads[i].place = i;
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
if(TEXT) printf("---PRZEŁĄCZANIE NA %d \n\n",i);
						cur_myThread_ptr=i;
						swapped = true;

						swapcontext(&mainThread.context,&All_myThreads[i].context);				
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
					if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND)
						{
if(TEXT)
printf("---PRZEŁĄCZANIE NA %d \n\n",i);
							swapped = true;
							int old = cur_myThread_ptr;
							cur_myThread_ptr=i;
							swapcontext(&All_myThreads[old].context,&All_myThreads[i].context);
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
					if(running_myThreads()==0) //done waiting
					{
if(TEXT)
printf("---PRZEŁĄCZANIE NA MAIN \n\n");
					swapped = true;
					int old = cur_myThread_ptr;
					cur_myThread_ptr=MAIN_THREAD;
					swapcontext(&All_myThreads[old].context,&mainThread.context);
					}
/*                                     */else if(TEXT)
printf("---MAIN wziąż czeka \n\n");
					
				}
				else if(mainThread.waitingFor == NOT_FOUND)
				{
if(TEXT) printf("---PRZEŁĄCZANIE NA MAIN \n\n");
					swapped = true;
					int old = cur_myThread_ptr;
					cur_myThread_ptr=MAIN_THREAD;
					swapcontext(&All_myThreads[old].context,&mainThread.context);
				}
					
			}
			if(!swapped)//checking myThreads first (0) to current
			{
if(TEXT) printf("--od 0 do obecnego\n");
				for(int i = 0; i < cur_myThread_ptr ; i ++)
					if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND)
						{
if(TEXT) printf("---PRZEŁĄCZANIE NA %d \n\n",i);
							swapped = true;
							int old = cur_myThread_ptr;
							cur_myThread_ptr=i;
							swapcontext(&All_myThreads[old].context,&All_myThreads[i].context);
						}
				
			}
		}
if(TEXT && swapped == false)
printf("---Nie było z czym zamienic\n");
	}
}

int Create_myThread(void (*function)(void), int id)
{
	if(id<1) return DONE_WRONG;
	int place = findFirstFree();
	if(place == NOT_FOUND) return DONE_WRONG;

	myThread newThread;
	getcontext(&newThread.context);
	newThread.context.uc_link = &cleanerThread;
	newThread.context.uc_stack.ss_sp = malloc(MY_THREAD_STACK_SIZE);
	newThread.stack = newThread.context.uc_stack.ss_sp;
	newThread.context.uc_stack.ss_size= MY_THREAD_STACK_SIZE;
	newThread.context.uc_stack.ss_flags = 0;
	if(newThread.context.uc_stack.ss_sp==0) return DONE_WRONG;

	newThread.isActive = true;
	newThread.waitingFor = -1;
	newThread.id=id;
	newThread.place = place;

	All_myThreads[place]=newThread;
	makecontext(&All_myThreads[place].context,function,0);

	return DONE_GOOD;
}
int Join_myThread(int idToJoin)// A join B => B !join A 
{
if(TEXT)
printf("\nJOIN who:%d to who:%d\n",cur_myThread_ptr,idToJoin);
	if(idToJoin >= 1)
		{
			int place = placeFromId(idToJoin);
			if(place!=NOT_FOUND)
			{	
				if(canI_JoinHim(cur_myThread_ptr,place)==DONE_GOOD)
				{
				All_myThreads[cur_myThread_ptr].waitingFor=place;
				schedule();
				return DONE_GOOD;
				}
			}
		}
	return DONE_WRONG;
}
int canI_JoinHim(int I, int Him)
{
	if(All_myThreads[Him].waitingFor == MAIN_THREAD) return DONE_GOOD;
	if(All_myThreads[Him].waitingFor == I) return DONE_WRONG;
	return canI_JoinHim(I,All_myThreads[All_myThreads[Him].waitingFor].waitingFor) ;
}
int placeFromId(int id)
{
	for(int i = 0; i < MTHREADS_NUM ; i ++)
		if(All_myThreads[i].id==id) return All_myThreads[i].place;	
	return NOT_FOUND;
}
void CleanThread()
{
if(TEXT) printf("\nCLEANER\n");
	isSomeoneWaitingFor(All_myThreads[cur_myThread_ptr].place);
if(TEXT) printf("%d \n",cur_myThread_ptr);
	All_myThreads[cur_myThread_ptr].isActive = false;
	free(All_myThreads[cur_myThread_ptr].stack);
	schedule();
}

 int WaitForAll_myThreads() 
{
	mainThread.waitingFor = MTHREADS_NUM; // waiting for all
	while(running_myThreads() != 0)      // wait until there are no active threads
		schedule();
	return DONE_GOOD;
	
}
int running_myThreads()
{
	int retVal = 0;
	for(int i = 0; i < MTHREADS_NUM ; i ++)
		if(All_myThreads[i].isActive == true) retVal++;
	
	return retVal;
}
int findFirstFree()
{
	for(int i = 0; i < MTHREADS_NUM ; i ++)
		if(All_myThreads[i].isActive == false) return i;
	return NOT_FOUND;
}

void isSomeoneWaitingFor(int Me)
{
	for(int i = 0; i < MTHREADS_NUM ; i ++)
		if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == Me)
			All_myThreads[i].waitingFor = NOT_FOUND;
	if(mainThread.waitingFor == Me) mainThread.waitingFor= NOT_FOUND;
}
//example 
void myThread_function()
{
	
	printf("WATEK POTOMNY 1 ROZPOCZYNA PRACE\n");
	schedule();
	for(int j = 0; j < 1; j ++)
		{
		printf("----------------%d WATEK 1 PRACUJE \n",j);

		schedule();
		}
	
	printf("WATEK POTOMNY 1 KONCZY PRACE\n");
	return;
}
void myThread_function2()
{
	printf("WATEK POTOMNY 2 ROZPOCZYNA PRACE\n");
	schedule();
	for(int j = 0; j < 40; j ++)
		{
		printf("----------------%d WATEK 2 PRACUJE\n",j);
		schedule();
		}
	Join_myThread(4);
	printf("WATEK POTOMNY 2 KONCZY PRACE\n");
	return;
}
void myThread_function3()
{
	printf("WATEK POTOMNY 3 ROZPOCZYNA PRACE\n");
	schedule();
	printf("WATEK POTOMNY 3 PRACUJE\n");
	schedule();
	printf("WATEK POTOMNY 3 PRACUJE 2\n");
	schedule();
	printf("WATEK POTOMNY 3 PRACUJE 3\n");
	schedule();

	
	printf("WATEK POTOMNY 3 KONCZY PRACE\n");
	return;
}
void myThread_function4()
{
	printf("WATEK POTOMNY 4 ROZPOCZYNA PRACE\n");
	schedule();
	printf("WATEK POTOMNY 4 PRACUJE\n");
	schedule();
	printf("WATEK POTOMNY 4 PRACUJE 2\n");
	schedule();
	printf("WATEK POTOMNY 4 PRACUJE 3\n");
	schedule();
	printf("WATEK POTOMNY 4 BEDZIE CZEKAL ZA WATKIEM 2\n");
	schedule();
	Join_myThread(2);

	printf("WATEK POTOMNY 4 SKONCZYŁ CZEKAĆ ZA WĄTKIEM 2\n");
	schedule();

	
	printf("WATEK POTOMNY 4 KONCZY PRACE\n");
	return;
}
int main(void)
{
	printf("Main thread\n");
	Init_myThreads();
	printf("New my thread is born:\n");
	if(Create_myThread(&myThread_function,1))
	printf("Error");
	if(Create_myThread(&myThread_function2,2))
	printf("Error");
	if(Create_myThread(&myThread_function3,3))
	printf("Error");
	if(Create_myThread(&myThread_function4,4))
	printf("Error");

	for(int j = 0; j < 10; j ++)
	{
	printf("----------------%d Main thread\n\n",j);

	schedule();
	}
	
	WaitForAll_myThreads();
	

	return 0;
}
