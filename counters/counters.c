/* 
 * counters.c - simple counters data structure
 *
 * Alex Wolf, April 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "counters.h"

 /*local type */
typedef struct countersData {
  int count;	// pointer to count
  int key;	// pointer to key (int>=0)
  struct countersData *next;   // other elements in the set(linked list)
} countersData_t;

//global type
typedef struct counters {
  struct countersData *first;   // first bag data element
} counters_t;

//create a new counters struct
counters_t *counters_new(void){

	counters_t *counters = malloc(sizeof(counters_t));

	if(counters==NULL){
		fprintf(stderr, "ERROR: counters sturct memory wasn't allocated properly\n");
		exit(1);
	}
	return counters;
}

//method to increae the counter by 1for a iven key
//if key not in counters init counterdata with count 1 and add to SLL
void counters_add(counters_t *ctrs, int key){

	//make sure key is int greater than or = to zero
	if(key<0){
		fprintf(stderr, "ERROR: Key in counters must be int greater than or equal to zero\n");
		exit (2);
	}

	countersData_t *counter= ctrs->first;
	countersData_t *counterData;
	countersData_t *prevCounter;

	//check if counters has no keys
	if(counter==NULL){

		countersData_t *counterData = malloc(sizeof(countersData_t));
		counterData->count=1; //init count to 1
		counterData->key=key;

		if(counterData==NULL){
			fprintf(stderr, "ERROR: counters sturct memory wasn't allocated properly\n");
			exit (1);
		}

		ctrs->first= counterData;

	}
	//there is data in counters already
	else{

		//loop through all data
		while(counter!=NULL){
				
			int dataKey= counter->key;

			//found key
			//increment count by 1
			if(dataKey==key){
				counter->count=counter->count+1;
				return;
			}
			prevCounter= counter;
			counter=counter->next;
		}

		//create new data with count 1 and append to end of SLL
		counterData = malloc(sizeof(countersData_t));
		counterData->count=1; //init count to 1
		counterData->key=key;

		if(counterData==NULL){
			fprintf(stderr, "counters sturct memory wasn't allocated properly\n");
			exit(1);
			}

		prevCounter->next= counterData;
	}
}

int counters_get(counters_t *ctrs, int key){

	//make sure key is int greater than or = to zero
	if(key<0){
		fprintf(stderr, "ERROR: Key in counters must be int greater than or equal to zero\n");
		exit (2);
	}

	countersData_t *counter= ctrs->first;

	//loop through all counters
	while(counter!=NULL){

		int dataKey= counter->key;

		//key found return count
		if(dataKey==key){
			return counter->count;
		} 

		counter= counter->next;
	}

	//return 0 (NULL) if key not found 
	return 0;
}

//delete counters struct and countersData structs within counters
void counters_delete(counters_t *ctrs){

	//get first counterdata
	countersData_t *counter= ctrs->first;

	//loop through all counters
	while(counter!=NULL){
		int count= counter->count;
		int key= counter->key;

		countersData_t *counterToFree= counter;
		counter= counter->next;

		//free single counter
		//don't need counter pointer since no void data being used
		free(counterToFree);

		if(counterToFree==NULL){
			fprintf(stderr, "ERROR: Couldn't free counterData properly\n");
			exit (3);
		}
		else{
			printf("Counter with key, %i, and count, %i, deleted propely\n", key, count);
		}
	}

	free(ctrs);
	if(ctrs==NULL){
			fprintf(stderr, "ERROR: Couldn't free ctrs properly\n\n");
			exit (3);
		}
		else{
			printf("counters struct, ctrs, deleted propely\n\n");
		}
}







