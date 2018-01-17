#include <iostream>
#include "myThread.h"
using namespace std;


int main(void)
{
	printf("Main thread\n");
	Init_myThreads();
	printf("New my thread is born:\n");
	Create_myThread(&myThread_function);
	WaitForAll_myThreads();
	
	return 0;
}
