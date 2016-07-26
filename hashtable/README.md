README for hashtable.c example
Alex Wolf, July 2016

Compiling:
	make

Usage:
	-Include hashtable.h in c program using
	-Make sure make file complies the hashtable.c object with whatever file using it
	-use functions as stated below to use hashtable struct

commmand:
	-call ,hashtabletest , to see struct working proplerly
 
  
  functions: 
 		hashtable_new();
 			- create a new hashtable data struct
 
  		hashtable_insert(hashtable, key, data)
 			-insert data with key into hashtable
 			-return true if inserted
 			-return false if key already in set
 
 		hashtable_find(ht, key)
 		 -find and return a data point based off existing key in hashtable
		 -return NULL(0) if key not found
 
 		hashtable_delete(hashtable);
 			-deletes all elemnts in hashtable
 			-prints statments if data delted sucessfully for testing
    
  output:
 		-none ecxcept when calling delete since it prints thing to prove testing
     

 stdin: None
 stdout: settest output
 stderr: error messages
 


Exit status:
  0 - success
  1 - hashtable wasn't intililzed correctly
  2 - couldn't delete data from memory properly



Assumptions:
-when inserting data puts a & before it (except for strings) to have a pointer to the data as void *

-converts data to type properly after calling extract


