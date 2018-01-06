/*
 * Task.h
 *
 *  Created on: Aug 22, 2017
 *      Author: pwnz
 */

#ifndef TASK_H_
#define TASK_H_
#include <time.h>
#include <stdlib.h>

// define a function pointer
typedef void*(*funcPtr)(void*);

class Task  {
public:

	funcPtr myFuncPtr;

	Task (const funcPtr fPtr) {
		myFuncPtr = fPtr;
	}

	~Task (){
		myFuncPtr = NULL;
	}

	void setFuncPtr(const funcPtr fPtr){
		this->myFuncPtr = fPtr;
	}

};

#endif /* TASK_H_ */
