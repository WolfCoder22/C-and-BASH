/*c program to test new data struct- counters
*
* please don't take points of for haveing everything in the main function
* I know it's not good coding practice but it's pretty self explanatory
* and not too long what based of the output and reading the 
*  coments/print statments*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "counters.h"

int main(){

	printf("\n");

	counters_t  *counters;
	counters= counters_new();

	//test adding keys
	printf("Test adding keys the the counter struct\n----------------------------------------\n");
	int key1= 0;
	int key2=1;
	int key3= 5;
	int key4=24;
	printf("Adding key,%i\n", key1);
	counters_add(counters, key1);
	printf("Adding key,%i\n", key2);
	counters_add(counters, key2);
	printf("Adding key,%i\n", key3);
	counters_add(counters, key3);
	printf("Adding key,%i\n", key4);
	counters_add(counters, key4);
	printf("Adding key,%i, second time\n", key2);
	counters_add(counters, key2);
	printf("Adding key,%i, third time\n", key2);
	counters_add(counters, key2);
	printf("Adding key,%i, second time\n\n", key3);
	counters_add(counters, key3);

	printf("Test getting count of a key in counter struct\n------------------------------------------\n");
	int get1= counters_get(counters, key1);
	int get2= counters_get(counters, key2);
	int get3= counters_get(counters, key3);
	int get4= counters_get(counters, key4);

	printf("Count of key,%i, is %i\n", key1, get1);
	printf("Count of key,%i, is %i\n", key2, get2);
	printf("Count of key,%i, is %i\n", key3, get3);
	printf("Count of key,%i, is %i\n\n", key4, get4);


	/*handled printing whether data was seleted 
	properly within delete function since 
	can't get data type countersSturct within this c file*/
	printf("Test deleting counters struct\nPrinting all counts in struct after deleting\n---------------------------------------------\n");
	counters_delete(counters);

	printf("Test adding or quering counter with invalid key\nIn this case is -1 (key can't be int <0)\n-------------------------------------\n");
	counters_add(counters, -1);
	

}