README for bag.c example
Alex Wolf, July 2016

Compiling:
	make

Usage:
	-Include bag.h in c program using
	-Make sure make file complies the bag.c object with whatever file using it
	-use functions as stated below to use bag struct

commmand:
	-call bagtest to see struct working proplerly
 
  
  functions: 
 		bag_new(free);
 			- create a new bag data struct
 
  		bag_insert(bag, data);
 			-insert data into bag
 
 		bag_extract(bag)
 		 - extract an element from the bag(in this case last inserted)
 		 -returns null if bag empty
 
 		bag_delete(bag_t *bag);
 			-deletes all elemnts in bag
 			-prints statments if data delted sucessfully for testing
    
  output:
 		-none ecxpt when calling delete since it prints thing to prove testing
     

 stdin: None
 stdout: bagtest output
 stderr: error messages
 


Example command lines:

	bagtest

Exit status:
  0 - success
  1 - bag wasn't intililzed correctly
  2 - couldn't delete data from memory properly



Assumptions:
-when inserting data puts a & before it (except for strings) to have a pointer to the data as void *

-!!!!!! Mallocs all data before inserting into bag so memory in stored in heap, not stack, so it is deleted properly

-converts data to type properly after calling extract

-reconginzes that null is the the same thing as returning 0 in extract_bag

-frees up memory from an extracted bagData if function extract_bag() called
