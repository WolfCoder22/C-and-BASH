/* 
* bagtest.c - test the functions of new struct bag
*
* please don't take points of for haveing everything in the main function
* I know it's not good coding practice but it's pretty self explanatory
* and not too long what based of the output and reading the 
*  coments/print statments
*
* Alex Wolf, April 2016
*/




#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "bag.h"

int main(){

	//create new bag with bag_delete function pointer as a parameter
	bag_t *bag=bag_new(free);

	//allocate memorory in heap for deleting later
 	char *data1 = (char *) malloc(sizeof(char *));
 	strcpy(data1, "Dartmouth");

 	char *data2 = (char *) malloc(sizeof(char *));
 	strcpy(data2, "@");


 	bag_insert(bag, data1);
 	bag_insert(bag, data2);

 	printf("Inserted -\"%s\" and '%s' into the bag\n\n", data1, data2);

 	//test extracting data propely
 	printf("Extrating all data now\n----------------------\n");
 	void *fromBag1= bag_extract(bag);
 	void *fromBag2= bag_extract(bag);

 	printf("\n");

 	printf("%s, is first data (string) extrated item from the bag.\n", (char *) fromBag1);
 	printf("%s, is third data (string) extrated item from the bag.\n\n", (char *) fromBag2);

 	//test retutn null when bag empty
 	printf("Test if extracting a item from an empty bag returns null\n--------------------------------------------------------\n");
 	//should return null
 	printf("The past extraction was null; the extration-%s\n", (char *) bag_extract(bag));

 	//reinsert data and delete bag
 	bag_insert(bag, data1);
 	bag_insert(bag, data2);


 	printf("Inserted -\"%s\", and '%s' into the bag\n\n", data1, data2);

 	//dete bag and it's contents
 	//prints if data deleted propely in bag_delete function in bag.c
 	bag_delete(bag);

 }


 	
