#include <iostream>
#include "myThread.h"
using namespace std;

void myThread_function()
{
	for(int i = 0; i < 4; i ++)
		printf("Hello, myThread there!\n");
}
int main(void)
{
	printf("Main thread\n");
	Init_myThreads();
	printf("New my thread is born:\n");
	Create_myThread(&function);
	WaitForAll_myThreads();
	
	return 0;
}
