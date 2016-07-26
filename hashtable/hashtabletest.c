/* c program to test new data struct- hashtable
*
* please don't take points of for haveing everything in the main function
* I know it's not good coding practice but it's pretty self explanatory
* and not too long what based of the output and reading the 
*  comments/print statments
*
*  Alex Wolf July, 2016      */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "hashtable.h"

//main function tests hashtable functions
int main(){

	printf("\n");


	//create hash table with 4 slots
	unsigned long  num_slots= 4;
	hashtable_t *hashtable= hashtable_new(num_slots);

	//init vars for testing
	char *data1= "Appple";
	char *data2= "Bannana";
	char *data3= "Pear";
	char *data4= "Stawberry";
	int data5= 24;
	int data6= 99;
	char *data7= "&@!#*";
	int data8= 42;

	char * key1= "aaba";
	char * key2= "weev";
	char * key3= "lol";
	char * key4= "lmfao";
	char * key5="rofl";
	char * key6="cs";
	char * key7="keyzin";
	char * key8="hashtables";
	char * unusedKey="XXX";

	printf("Test inseting data into hashtable with different keys\n------------------------------------------------------\n");
	if(hashtable_insert(hashtable, key1, data1)!=0){
		printf("Data -%s- inserted sucessfully in hashtable with key %s\n", data1, key1);
		//printf("here");
	}

	if(hashtable_insert(hashtable, key2, data2)!=0){
		printf("Data -%s- inserted sucessfully in hashtable with key %s\n", data2, key2);
	}
	if(hashtable_insert(hashtable, key3, data3)!=0){
		printf("Data -%s- inserted sucessfully in hashtable with key %s\n", data3, key3);
	}
	if(hashtable_insert(hashtable, key4, data4)!=0){
		printf("Data -%s- inserted sucessfully in hashtable with key %s\n", data4, key4);
	}
	if(hashtable_insert(hashtable, key5, (void *) &data5)!=0){
		printf("Data -%i- inserted sucessfully in hashtable with key %s\n", data5, key5);
	}
	if(hashtable_insert(hashtable, key6, (void *) &data6)!=0){
		printf("Data -%i- inserted sucessfully in hashtable with key %s\n", data6, key6);
	}
	if(hashtable_insert(hashtable, key7, data7)!=0){
		printf("Data -%s- inserted sucessfully in hashtable with key %s\n", data7, key7);
	}
	if(hashtable_insert(hashtable, key8, (void *) &data8)!=0){
		printf("Data -%i- inserted sucessfully in hashtable with key %s\n\n", data8, key8);
	}

	printf("Test inseting data into hashtable that already exists\n------------------------------------------------------\n");
	if(hashtable_insert(hashtable, key1, data1)==0){
		printf("Data -%s- already exists in hashtable with key %s\n", data1, key1);
		//printf("here");
	}
	if(hashtable_insert(hashtable, key5, (void *) &data5)==0){
		printf("Data -%i- already exists in hashtable with key %s\n\n", data5, key5);
	}

	printf("Test finding data into hashtable that exists\n---------------------------------------------\n");
	char *foundData1=hashtable_find(hashtable, key1);
	int foundData2= *(int *)hashtable_find(hashtable, key5);
	char * nullData= hashtable_find(hashtable, unusedKey);

	printf("%s, is data found in hashtable from key %s\n",foundData1, key1);
	printf("%d, is data found in hashtable from key %s\n",foundData2, key2);
	printf("%s, is data found in hashtable from key %s\n",nullData, unusedKey);

	printf("Test deleting the hashtable\n---------------------------\n");
	hashtable_delete(hashtable);

}