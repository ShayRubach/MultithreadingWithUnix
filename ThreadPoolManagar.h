/*
 * ThreadPoolManagar.h
 *
 *  Created on: Aug 22, 2017
 *      Author: pwnz
 */

#ifndef THREADPOOLMANAGAR_H_
#define THREADPOOLMANAGAR_H_
#include <vector>
#include <exception>

#define GREEN	"\x1B[32m"
#define WHITE  	"\x1B[0m"

using std::cout;
using std::endl;


class ThreadPoolManagar {

private:
	int maxThreads;
	int ratio;
	pthread_t* threadPool;			// threads
	bool*      threadStatus;		// unoccupied (0) / occupied (1)

public:
	funcPtr myFuncPtr;
	pthread_t  masterThread;

	ThreadPoolManagar(int size,int ratio){
		setMaxThreads(size);
		setRatio(ratio);
		if(size){
			threadPool = new pthread_t[size];							// array of threads
			threadStatus = new bool[size];
			clearStatus();

			if(!threadPool || !threadStatus){
				cout << "Failed to allocate thread pool." << endl;
				exit(-1);
			}
		}
	}

	int request(){
		for(int i=0; i < maxThreads ;++i){
			if(threadStatus[i] == 0){
				return i;
			}
		}
		return -1;														// no available threads
	}


	pthread_t& operator[](int index){
		return threadPool[index];
	}

	pthread_t& getThread(int index){
			return threadPool[index];
	}

	void joinAll(){
		for(int i = 0;  i < maxThreads ; ++i){
			if(threadStatus[i]){
				pthread_join(threadPool[i],NULL);
			}
		}
	}


	inline virtual ~ThreadPoolManagar() {
		if(threadPool){
			delete[] threadPool;
		}
		if(threadStatus){
			delete[] threadStatus;
		}
	}

	inline const pthread_t* getThreadPool() const {
		return threadPool;
	}

	int& getRatio() {
		return ratio;
	}

	void setRatio(int iterationRatio) {
		this->ratio = iterationRatio;
	}

	int getMaxThreads() const {
		return maxThreads;
	}

	void setMaxThreads(int maxThreads) {
		this->maxThreads = maxThreads;
	}

	const bool getStatus(int index) const {
		return threadStatus[index];
	}

	void setStatus(int index,int value) {
		threadStatus[index] = value;;
	}

	void clearStatus(){
		std::fill(threadStatus, threadStatus+maxThreads, 0);
		cout << GREEN "Cleared. All threads are now available." WHITE << endl;
	}
};





#endif /* THREADPOOLMANAGAR_H_ */
