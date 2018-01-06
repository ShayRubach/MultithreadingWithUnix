/*
 * TastFeeder.h
 *
 *  Created on: Aug 22, 2017
 *      Author: pwnz
 */

#ifndef TASKFEEDER_H_
#define TASKFEEDER_H_
#include "ThreadPoolManagar.h"
#include <queue>

using std::queue;
using std::cout;
using std::endl;

class TaskFeeder {

private:
	queue<Task> tasksList;
	ThreadPoolManagar* manager;

public:
	TaskFeeder() :
		manager(0)
	{}

	TaskFeeder(ThreadPoolManagar& _manager){
		setManager(_manager);
	}

	virtual ~TaskFeeder() {}


	Task pop(){
		if(!this->tasksList.empty()){
			Task task = tasksList.front();
			try {
				tasksList.pop();
			}
			catch (...) {
				cout << "Error: tasksList.pop()" << endl;
			}
			return task;
		}
	}

	Task front(){
		try {
			if(!this->tasksList.empty()){
				return tasksList.front();
			}
		}
		catch (...) {
			cout << "Error: tasksList.front()" << endl;
		}
	}


	inline void addTask(Task task){
		try {
			tasksList.push(task);
		}
		catch (...) {
			cout << "Error: tasksList.push()" << endl;
		}
	}

	inline void removeTask(){
		try {
			tasksList.pop();
		}
		catch (...) {
			cout << "Error: tasksList.pop()" << endl;
		}
	}

	inline bool isEmpty() const {
		return tasksList.empty();
	}

	inline const queue<Task>& getTasksList() const {
		return tasksList;
	}

	inline int size() const {
		return tasksList.size();
	}

	inline void setManager(const ThreadPoolManagar& manger){
		this->manager = manager;
		if(this->manager){
			cout << "in TaskFeeder: manager set to null" << endl;
		}
	}

};





#endif /* TASTFEEDER_H_ */
