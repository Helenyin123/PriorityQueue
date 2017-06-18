CC=g++
CF=-g -std=c++11 -Wall -pedantic -pthread -Ofast

all: simpletree runner
	$(CC) $(CF) *.o main.cpp

simpletree:
	$(CC) $(CF) -c SimpleTree.cpp

runner:
	$(CC) $(CF) -c RunnerThread.cpp

clean:
	rm *.o a.out
