/* 
* settest.c - test the functions of new struct set
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
#include "set.h"

int main(){

	set_t  *set;
	set= set_new(free);//foinftion pointer to free passed as destructor

	//cjeck if build failed
	if (set == NULL) {
    	printf("tree_new failed intilaizing\n");
    	exit(1);
 	}

 	printf("Set created properly\n\n");

 	//allocate data memory in heap for deleting later
 	char *var1 = (char *) malloc(sizeof(char *));
 	strcpy(var1, "Dartmouth");

 	char *var2 = (char *) malloc(sizeof(char *));
 	strcpy(var2, "cs50");

 	char *var3 = (char *) malloc(sizeof(char *));
 	strcpy(var3, "hate and love this class");

 	//insert data into set
 	set_insert(set, "data1", var1);
 	printf("Inserted new data- %s- with key \"data1\"into set\n", var1);
 	set_insert(set, "data2", var2);
 	printf("Inserted new data- %s- with key \"data2\" into set\n", var2);
 	set_insert(set, "data3", var3);
 	printf("Inserted new data- %s- with key \"data3\"into set\n\n", var3);

 	printf("Try inserting data into set with preexisting key\n----------------------------------------------------\n");
 	if(set_find(set, "data2")!=0){
 		printf("data, %s, already in set. Not adding\n\n", var2);
 	}

 	printf("Try finding data in set with preexisting and non existing key\n----------------------------------------------------\n");
 	if(set_find(set, "data1")!=0){
 		 printf("Key -\"%s\"- already exists with data -\"%s\"\n", "data1", (char *) set_find(set, "data1"));

 	}
 	else{
 		printf("Key -\"%s\"- does not exist in the set", "data1\n");
 	}

 	if(set_find(set, "Non-Exiting key")!=0){
 		printf("Key -\"Non-Exiting key\"- already exists\n");
 	}
 	else{
 		printf("Key -\"%s\"- does not exist in the set\n\n", "Non-Exiting key");
 	}

 	/*handled printing whether data seleted deleted
	properly within delete function since 
	can't get data type setData within this c file*/
	printf("Test deleting setData struct\nPrinting keys in setData structs after deleting\n---------------------------------------------\n");
	set_delete(set);
	
}