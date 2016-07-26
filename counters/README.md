README for counterstest.c example
Alex Wolf, July 2016

Compiling:
	make

Usage:
	-Include sounters.h in c program using
	-Make sure make file complies the counters.c object with whatever file using it
	-use functions as stated below to use bag struct

	functions:
		counters_new(void);
			-inite a new counter object
		counters_add(ctrs, key);
			-method to increase the counter by 1 for a given key
				-key must key integer greater than or equal to 0 or error
			-if key not in counters, inits single counter with count 1
		counters_get(ctrs, key);
			-get the count of a given key
				-key must key integer greater than or equal to 0 or error
		counters_delete(ctrs);
			-delete counters stuct intied and all data in it in memory
 
 usage: call counterstest to see functions in action
 output:
 		Prints the various reults from testing fthe fucntions of counter
     
 stdin: None
 stdout: testing outpouts of set
 stderr: error messages
 
 

Example command lines:

  - after init counter stuct with counters_t ctrs= counters_new(); c
  -use that object in fucntions associtted with it after to make it work

Exit status:
  0 - success
  1 - counter mem wasn't allocated correctly
  2 - invalid key entered when quering or adding
  3- couldn't free data propely


Assumptions:
-The user knows to delete the print statements in the counters_delete function
 since it is only used for testing it. Wouldn't want to print unessary stuff 
 because functionalready works

