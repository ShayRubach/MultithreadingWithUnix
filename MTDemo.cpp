//============================================================================
// Name        : MTDemo.cpp
// Author      : ShayRubach (305687352) & ReutLeib (302764576)
// Version     : 1.0
// Copyright   : Your copyright notice
// Description : Multi Threading Demo
//============================================================================

#include <iostream>
#include <sys/time.h>
#include <pthread.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
#include "Task.h"
#include "TaskFeeder.h"
#include "ThreadPoolManagar.h"
#include "Functions.h"


using namespace std;

int main(int argc,char** argv) {

	totalTasksCount = 0;
	ratioSum = 0;
	ThreadData* data = new ThreadData();
	ThreadControl* threadControl = new ThreadControl;
	TaskFeeder taskFeeder;		
	string formattedMsg;													
	struct timeval start, end;

	printTimeStamp();
	validateArgs(argc,argv);													// validate arguments recieved from client
	gettimeofday(&start, NULL);

	ThreadPoolManagar threadPoolManager (atoi(argv[1]),atoi(argv[2]));			// thread pool manager

	taskFeeder.setManager(threadPoolManager);									// bind manager to feeder
	data->ratio = &(threadPoolManager.getRatio());								// save  pointer to ratio - reading convenience
	*(data->ratio) = (*(data->ratio)) / cpuCount;								// fixed ratio for each thread


	threadControl->manager = &threadPoolManager;								// init thread control
	threadControl->feeder = &taskFeeder;
	threadControl->data = data;

	pthread_create(&(threadPoolManager.masterThread)							// start the master thread:
			,NULL,&(doTasks)													// this thread will a-syncly create
			,(void*)threadControl);												// threads for the tasks by demand



	for(int i=0; i < cpuCount ; ++i,++totalTasksCount){							// create N tasks for Pi calculation
		pthread_mutex_lock(&poolLock);
		logMsg("Added a new task of type: [Pi Calculation] \n");
		taskFeeder.addTask(Task(&(calculatePi)));
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&poolLock);
	}


	for(int i=0; i < cpuCount ; ++i,++totalTasksCount){							// create N tasks for Ratio Sum calculation
		pthread_mutex_lock(&poolLock);
		logMsg("Added a new task of type: [Ratio Sum] \n");
		taskFeeder.addTask(Task(&(countToRatio)));
		pthread_cond_signal(&cond);
		pthread_mutex_unlock(&poolLock);
	}

	pthread_join(threadPoolManager.masterThread,NULL);							// let the manager finish his work

	ratio(ratioSum);															// print calculated Ratio
	double pi = average(data->results);											// print calculated average Pi
	gettimeofday(&end, NULL);
	double elapsed = ((end.tv_sec - start.tv_sec) * 1000)
					+ (end.tv_usec / 1000 - start.tv_usec / 1000);

	formattedMsg = toString(pi);												// print results & total runtime:
	logMsg("\nAvaraged Pi result: " + formattedMsg + "\n");						// to log
	formattedMsg = toString(elapsed/1000);
	logMsg("\nTotal runtime: " + formattedMsg + "\n");

	cout << fixed;
	cout << setprecision(3);
	cout << CYAN "Total runtime: " WHITE << elapsed / 1000 						// to console
		 << " seconds."  << endl << endl;
	return 0;
}
