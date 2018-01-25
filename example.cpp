#include <iostream>
#include "myThread.h"
using namespace std;
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
	
	printf("Main thread kończy pracę\n");
	return 0;
}
