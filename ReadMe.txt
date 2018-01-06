** Shay Rubach 305687352, Reut Leib 302764576 **

				------------------
   				 Multhi-threading
   				------------------
****************************************************************
****PLEASE VIEW THIS FILE IN FULLSCREEN FOR BEST IDENTATIONS****
****************************************************************


==============
==Disclaimer==
==============

- Code was written in C++ and compiled with g++
- Makefile was edited in SublimeText2 (our TextEditor yielded invisible spaces)
- If there was a use of opensource code, copyrights are mentioned
- Our program determines how many cores the CPU has and divide and conquer by cores. If you want to test out the parallelism at his best, we recommand changing 'cpuCore' global variable [file: Functions.h] to 10+ and higher.
- Our program demonstrates 2 DIFFERENT tasks executions, the requested Pi by MonteCarlo & another RatioCount (just for the sake of generic approach). 
- Pi by MonteCalro code is taken from: https://www.youtube.com/watch?v=adDY7k1Hsqg

============
===How to===
============

To use our program, please follow this instructions:

0. run makefile					(cd to pwd, make clean + make )
1. start exe file MTDemo		(./MTDemo {0} {1} ) - when {0} = maximum threads in pool , {1} = iterations of MonteCarlo / RatioSum functions.


============
==Approach==
============

- We created a father class 'Task'. Task holds a function pointer to later be passed to the pthread_create() function.
- In order to use any kind of Task and feed it to the pool, you only need to instantiate a Task and pass the function pointer you wish to work with (depending on the function you wish to execute) to the constructor, or use the appropriate setter.
- We have provided a "Functions.h" header files which contains/could contain any sort of function to be used in our program. In our program we will demonstrate an invoke of Pi Calculation & Ratio Sum in order to prove the generic approach we took.

*All functions are under the restricted signature:

void*			foo		(	void* arg	)
[return type]	[name]	[parameter type]


- This condition is a must in order to successfully compile and run the program. So, if you wish to add another sort of task to be executed by the pool, just follow this signature.



