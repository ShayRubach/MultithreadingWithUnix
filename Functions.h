/*
 * Functions.h
 *
 *  Created on: Aug 25, 2017
 *      Author: pwnz
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#define RED		"\x1B[31m"					// console text color ascii code:
#define GREEN	"\x1B[32m"
#define YELLOW	"\x1B[33m"
#define BLUE	"\x1B[34m"
#define WHITE  	"\x1B[0m"
#define GREY	"\x1B[37m"
#define CYAN	"\x1B[36m"

#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <fstream>
#include "ThreadPoolManagar.h"

using namespace std;

extern pthread_mutex_t poolLock = PTHREAD_MUTEX_INITIALIZER;
extern pthread_mutex_t dataLock = PTHREAD_MUTEX_INITIALIZER;
extern pthread_cond_t  cond  =	PTHREAD_COND_INITIALIZER;

extern const char* fileName = "log.txt";
extern int cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
extern int totalTasksCount = 0;
extern int ratioSum = 0;

typedef struct{								// passed as a parameter to generic function
	int* ratio;
	int  workingThreadIndex;
	std::vector<double> results;
}ThreadData ;

typedef struct{								// passed as a parameter to generic function
	ThreadPoolManagar* 	manager;
	TaskFeeder* 		feeder;
	ThreadData* 		data;
}ThreadControl;


template <typename T>
string toString(const T& number){
    std::stringstream ss;
    ss << number;
    return ss.str();
}


void logMsg(string formattedMsg){
	fstream logFileHandle;
	logFileHandle.open(fileName,fstream::app);
	logFileHandle << formattedMsg;
	logFileHandle.close(); 
}


//void logLocationMessage(string file, string func){
//	fstream logFileHandle;
//	logFileHandle.open(fileName,fstream::app);
//
//	string formattedMsg;
//	formattedMsg.append("\t\t[ ");
//	formattedMsg.append(file);
//	formattedMsg.append("( function : ");
//	formattedMsg.append(func);
//	formattedMsg.append(") ]");
//	formattedMsg.append("\n");
//
//	logFileHandle << formattedMsg;
//	logFileHandle.close();
//}

// this method is taken from: https://www.youtube.com/watch?v=adDY7k1Hsqg
void* calculatePi(void* arg){


	ThreadControl* controller = (ThreadControl*)arg;
	int currentThreadIndex = controller->data->workingThreadIndex;
	double 	pi = 0;
	double 	sum = 0;
	int iterations = *(controller->data->ratio);
	int array[iterations][2];
	string formattedID = toString(pthread_self());

	logMsg("ThreadID: [" + formattedID + "] -> started calculating Pi..\n");

	srand(time(NULL));


	for(int i = 0 ; i < iterations ; ++i){
		array[i][1] = rand()%101;
		array[i][0] = rand()%101;
	}

	for(int j = 0 ; j < iterations ; ++j){
		if((pow(array[j][0],2) + pow(array[j][1],2) ) <= 10000){
			++sum;
		}
	}

	pi = sum / iterations * 4;

	pthread_mutex_lock(&dataLock);
	controller->data->results.push_back(pi);									// push the calculated result
	logMsg("ThreadID: [" + formattedID + "]  pushed a result \n");
	controller->manager->setStatus(currentThreadIndex,0);						// done. set current thread as occupied
	logMsg("ThreadID: [" + formattedID + "]  finished his job.! \n");
	pthread_mutex_unlock(&dataLock);

	return (void*)arg;
}

void* countToRatio(void* arg){													// count and sum the ratio recieved as
	ThreadControl* controller = (ThreadControl*)arg;							// argv[2] using multipile threads
	int ratio = *(controller->data->ratio);
	string formattedID = toString(pthread_self()); 


	logMsg("ThreadID: [" + formattedID + "] -> started counting toRatio.. \n");
	
	for(int i=0; i < ratio ; ++i){
		pthread_mutex_lock(&poolLock);											// lock the pool to avoid ghost inserts
		ratioSum++;
		pthread_mutex_unlock(&poolLock);
	}

	return (void*)arg;
}

void validateArgs(int argc,char** argv){										// validate arguments recieved
	if(argc != 3){
		cout << "You " RED "MUST" WHITE " provide 2 arguments: "
				GREEN "{Maximum threads}" WHITE ","
				GREEN " {Maximum iterations per thread}" WHITE << endl;
		exit(-1);
	}
	cout << endl;
	cout << GREY "Maximum  threads: " YELLOW << argv[1] << WHITE << endl;
	cout << GREY "Iterations ratio: " YELLOW << argv[2] << WHITE << endl;

	logMsg("Arguments are valid. \n\n");
	// to avoid buffer overflow and segmantation faults
	if(atoi(argv[2]) > 1000000){
		strcpy(argv[2],"1000000");
	}

	if(atoi(argv[1]) < cpuCount){
		cpuCount = atoi(argv[1]);
	}

}

double average(std::vector<double> results){
	double averagedResult = 0;
	int	size = results.size();

	while(!results.empty()){
		averagedResult += results.back();
		results.pop_back();
	}
	cout << std::fixed;
	cout << std::setprecision(5);
	cout << YELLOW "====================================" << endl;
	cout << YELLOW "==  "GREY"Averaged calculation: "	GREEN;
	cout << averagedResult / size << 		 YELLOW " ==" << endl;
	cout << YELLOW "====================================" WHITE << endl << endl;
	return averagedResult / size;
}

void ratio(int ratioSum){
	cout << endl;
	cout << BLUE "====================================" << endl;
	cout << BLUE "==  "GREY"Ratio summed to: "	BLUE;
	cout << ratioSum << 		 BLUE "  " << endl;
	cout << BLUE "====================================" WHITE << endl << endl;
}


void* doTasks(void* arg){
	int freeThreadIndex = -1;
	int tasksCompleted = 0;
	ThreadControl* controller = (ThreadControl*)arg;
	
	// tolog
	logMsg("MasterThread: Starting tasks execution.. \n");

	while(tasksCompleted++ != totalTasksCount){								// while all tasks are still unfinished
		pthread_mutex_lock(&poolLock);
		while(controller->feeder->isEmpty()){
			pthread_cond_wait(&cond,&poolLock);
		}
																			// ::::LOGIC STARTS HERE::::
		do {																// get a free thread. if none available, keep waiting.
			freeThreadIndex = controller->manager->request();
		}
		while(-1 == freeThreadIndex);

		controller->data->workingThreadIndex = freeThreadIndex;
		controller->manager->setStatus(freeThreadIndex,1);					// set thread status to occupied
		pthread_create(&(controller->manager->getThread(freeThreadIndex))	// handle the task with a new thread from pool
				,NULL
				,controller->feeder->front().myFuncPtr
				,(void*)controller
				);

		// tolog
		logMsg("New thread created. \n");		
		
		controller->feeder->removeTask();
		pthread_mutex_unlock(&poolLock);									// ::::LOGIC ENDS HERE::::
	}
	controller->manager->joinAll();											// make sure we finished before exiting...
	return arg;
}



void printTimeStamp(){
	fstream logFileHandle;
	logFileHandle.open(fileName,fstream::out);
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	// logFileHandle << (now->tm_mon + 1) << '-' <<  now->tm_mday << now->tm_min<< now->tm_sec << endl;
	logFileHandle.close();
}




#endif
