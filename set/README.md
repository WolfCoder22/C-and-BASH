README for set.c example
Alex Wolf, July 2016

Compiling:
	make

Usage:
	-Include set.h in c program using
	-Make sure make file complies the set.c object with whatever file using it
	-use functions as stated below to use set struct

commmand:
	-call ,settest , to see struct working proplerly
 
  
  functions: 
 		set_new(free);
 			- create a new set data struct
 
  		set_insert(set, key, data);
 			-insert data with key into set
 			-return true if inserted
 			-return false if key already in set
 
 		set_find(set, key)
 		 -find and return a data point based off existing key
		 -return NULL(0 and false same thing) if key not found
 
 		set_delete(set_t *set);
 			-deletes all elemnts in set
 			-prints statments if data delted sucessfully for testing
    
  output:
 		-none ecxpt when calling delete since it prints thing to prove testing
     

 stdin: None
 stdout: settest output
 stderr: error messages
 


Exit status:
  0 - success
  1 - set wasn't intililzed correctly
  2 - couldn't delete data from memory properly



Assumptions:
-when inserting data puts a & before it (except for strings) to have a pointer to the data as void *

-!!!!!! Mallocs all data before inserting into bag so memory in stored in heap, not stack, so it is deleted properly

-converts data to type properly after calling extract

