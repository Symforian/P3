myThread: 
	g++ -Wall -Werror -o myThread.o myThread.cpp

example: myThread
	g++ -Wall -Werror -o example.o example.cpp
run: myThread.o
	./myThread.o
clean:
	rm *.o


