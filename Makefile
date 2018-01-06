all: MTDemo.o 
	g++ MTDemo.o -o MTDemo -pthread
MTDemo: MTDemo.cpp
	g++ -c MTDemo.cpp -o MTDemo.o
ThreadPoolManagar: ThreadPoolManagar.h
	g++ -c ThreadPoolManagar.h -o ThreadPoolManagar.o
TaskFeeder: TaskFeeder.h
	g++ -c TaskFeeder.h -o TaskFeeder.o
Task: Task.h
	g++ -c Task.h -o Task.o
Functions: Functions.h
	g++ -c Functions.h -o Functions.o
clean:
	rm -rf MTDemo.o Task.o TaskFeeder.o ThreadPoolManagar.o Functions.o