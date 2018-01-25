#include "myThread.h"
#include <time.h>
#include <unistd.h>
  //all threads list
  myThread All_myThreads [MTHREADS_NUM];
int cur_myThread_ptr = -1;
int maxtime; 
int actualTime;
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
	actualTime = clock();
	maxtime = clock()+TIMEINT;

}
void schedule()
{
	actualTime = clock();

	if(actualTime>maxtime || All_myThreads[cur_myThread_ptr].isActive == false || All_myThreads[cur_myThread_ptr].waitingFor==SEM_LOCK_SIG)
	{
		bool swapped = false;

		if(cur_myThread_ptr == MAIN_THREAD) //We were currently doing main
		{
	
			for(int i = 0; i < MTHREADS_NUM ; i ++)
			{	


				if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND)
					{
						cur_myThread_ptr=i;
						swapped = true;

						swapcontext(&mainThread.context,&All_myThreads[i].context);				
					}
			}
		}
		else
		{
			if(!swapped)//checking myThreads current+1 to last one if ready 
			{
				for(int i = cur_myThread_ptr+1; i < MTHREADS_NUM ; i ++)
					if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND)
						{
							swapped = true;
							int old = cur_myThread_ptr;
							cur_myThread_ptr=i;
							swapcontext(&All_myThreads[old].context,&All_myThreads[i].context);
						}
				
			}
			if(!swapped)//swapping with main
			{	
				if(mainThread.waitingFor == MTHREADS_NUM) //main is waiting for them all
				{
					if(running_myThreads()==0) //done waiting
					{
					swapped = true;
					int old = cur_myThread_ptr;
					cur_myThread_ptr=MAIN_THREAD;
					swapcontext(&All_myThreads[old].context,&mainThread.context);
					}
				}
				else if(mainThread.waitingFor == NOT_FOUND)
				{
					swapped = true;
					int old = cur_myThread_ptr;
					cur_myThread_ptr=MAIN_THREAD;
					swapcontext(&All_myThreads[old].context,&mainThread.context);
				}
					
			}
			if(!swapped)//checking myThreads first (0) to current
			{
				for(int i = 0; i < cur_myThread_ptr ; i ++)
					if(All_myThreads[i].isActive == true && All_myThreads[i].waitingFor == NOT_FOUND)
						{
							swapped = true;
							int old = cur_myThread_ptr;
							cur_myThread_ptr=i;
							swapcontext(&All_myThreads[old].context,&All_myThreads[i].context);
						}
				
			}
		}
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
	
	if(idToJoin >= 1)
		{
			int place = placeFromId(idToJoin);
			if(place!=NOT_FOUND)
			{	
				if(cur_myThread_ptr==MAIN_THREAD) mainThread.waitingFor=place;
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
	isSomeoneWaitingFor(All_myThreads[cur_myThread_ptr].place);
	All_myThreads[cur_myThread_ptr].isActive = false;
	All_myThreads[cur_myThread_ptr].id = 0;
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

//semaphore
void Init_mySemaphore(mySemaphore* sem, int amount)
{
	sem->counter=amount;
	for(int i = 0; i< MTHREADS_NUM; i++)
	sem->IdQueue[i]=-1;
}
//wait 
void wait_myThread(mySemaphore* sem)
{
	if(sem->counter>0)
	{
	sem->counter--;
	All_myThreads[cur_myThread_ptr].waitingFor = SEM_LOCK_SIG;
	sem->IdQueue[sem->counter]=All_myThreads[cur_myThread_ptr].id;
	}
	
}
//signal
void signal_myThread(mySemaphore* sem)
{
	if(sem->counter>=0)
	{
	All_myThreads[placeFromId(sem->IdQueue[sem->counter])].waitingFor = MAIN_THREAD;
	sem->IdQueue[sem->counter]=-1;
	sem->counter++;
	}
}
//example 
/////////////////////////////////////////////////////////
void myThread_function()
{
	
	printf("WATEK POTOMNY 1 ROZPOCZYNA PRACE\n");
	schedule();
	for(int j = 0; j < 5; j ++)
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
	printf("WATEK POTOMNY 2 BEDZIE CZEKAL ZA WATKIEM 4\n");
	if(Join_myThread(4)==DONE_WRONG)
		printf("Nie można było czekać (2 za 4)\n");
	else
	printf("WATEK POTOMNY 2 SKONCZYŁ CZEKAĆ ZA WĄTKIEM 4\n");
	
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
	if(Join_myThread(2)==DONE_WRONG)
		printf("Nie można było czekać (4 za 2)\n");
	else
	printf("WATEK POTOMNY 4 SKONCZYŁ CZEKAĆ ZA WĄTKIEM 2\n");
	schedule();

	
	printf("WATEK POTOMNY 4 KONCZY PRACE\n");
	return;
}
/////////////////////////////////////////////////////////
void myThread_function5()
{
	printf("Wątek 1 zaczyna pracę\n");
	schedule();
	for(int i=0; i < 15 ; i++)
	{
		printf("Zaraz zaśnie (W1)\n");
		schedule();
		usleep(40);
		schedule();
	}
	printf("Wątek 1 kończy pracę\n");
	return;
}
void myThread_function6()
{
	printf("Wątek 2 zaczyna pracę\n");
	schedule();
	for(int i=0; i < 15 ; i++)
	{
		printf("Zaraz zaśnie (W2)\n");
		schedule();
		usleep(50);
		schedule();
	}
	printf("Wątek 2 kończy pracę\n");
	return;
}
void myThread_function7()
{
	printf("Wątek 3 zaczyna pracę\n");
	schedule();
	printf("Zaraz zaśnie (W1)\n");
	schedule();
	usleep(100000);
	schedule();
	printf("(W3) Zaraz poczeka na (W2)\n");
	schedule();
	Join_myThread(2);
	schedule();
	printf("Wątek 3 kończy pracę\n");
	return;
}
/////////////////////////////////////////////////////////
mySemaphore lock;
void myThread_function8()
{
	printf("Wątek 1 zaczyna pracę\n");
	schedule();
	printf("Zaraz wejście w sekcje krytyczną\n");
	schedule();
	wait_myThread(&lock);
	schedule();
	printf("Jesteśmy w sekcji krytycznej (W1)\n");
	schedule();
	usleep(100000);
	schedule();
	printf("Opuszczamy sekcje krytyczną (W1)");
	schedule();	
	signal_myThread(&lock);
	printf("Wątek 1 kończy pracę\n");
	return;
}
void myThread_function9()
{
	printf("Wątek 2 zaczyna pracę\n");
	schedule();
	printf("Zaraz wejście w sekcje krytyczną\n");
	schedule();
	wait_myThread(&lock);
	schedule();
	printf("Jesteśmy w sekcji krytycznej (W2)\n");;
	schedule();
	printf("Wątek 2 kończy pracę\n");
	return;
}
void myThread_function10()
{
	printf("Wątek 3 zaczyna pracę\n");
	schedule();
	printf("Zaraz wejście w sekcje krytyczną\n");
	schedule();
	wait_myThread(&lock);
	schedule();
	printf("Jesteśmy w sekcji krytycznej (W3)\n");;
	schedule();
	printf("Wątek 3 kończy pracę\n");
	return;
}
/////////////////////////////////////////////////////////
int main(void)
{
	printf("Main thread\n");
	Init_myThreads();
	int test1=1;
	int test2=1;
	int test3=0;//DOES NOT WORK
	if(test1)
{
	printf("------------------------------TEST 4 wątki:\n");
	if(Create_myThread(&myThread_function,1))
	printf("Error");
	schedule();
	if(Create_myThread(&myThread_function2,2))
	printf("Error");
	schedule();
	if(Create_myThread(&myThread_function3,3))
	printf("Error");
	schedule();
	if(Create_myThread(&myThread_function4,4))
	printf("Error");
	schedule();
	for(int j = 0; j < 10; j ++)
	{
	printf("----------------%d Main thread\n\n",j);
	schedule();
	}
	printf("Zaraz main poczeka na resztę\n");
	schedule();
	WaitForAll_myThreads();
	printf("------------------------------KONIEC TEST 4 wątki\n");
}
	if(test2)
{
	printf("------------------------------TEST czasowy wątki:\n");
	if(Create_myThread(&myThread_function5,1))
	printf("Error");
	schedule();
	if(Create_myThread(&myThread_function6,2))
	printf("Error");
	schedule();
	if(Create_myThread(&myThread_function7,3))
	printf("Error");
	schedule();
	printf("Zaraz main poczeka na W(3)\n");
	Join_myThread(3);
	printf("Koniec czekania na W3\n");
	schedule();
	printf("Zaraz main poczeka na resztę\n");
	schedule();
	WaitForAll_myThreads();
	printf("------------------------------KONIEC TEST czasowy wątki:\n");
}
	if(test3)
{
	printf("------------------------------TEST semafory:\n");
	Init_mySemaphore(&lock,3);
	if(Create_myThread(&myThread_function8,1))
	printf("Error");
	schedule();
	if(Create_myThread(&myThread_function9,2))
	printf("Error");
	schedule();
	if(Create_myThread(&myThread_function10,3))
	printf("Error");
	printf("Zaraz main poczeka na resztę\n");
	schedule();
	usleep(3000000);
	signal_myThread(&lock);
	printf("Zaraz main poczeka na resztę\n");
	schedule();
	WaitForAll_myThreads();
	printf("------------------------------TEST semafory:\n");
}		
	printf("Main thread kończy pracę\n");
	return 0;
}
