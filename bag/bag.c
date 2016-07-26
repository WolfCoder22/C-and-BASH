/* 
 * bag.c - simple bag module
 *  (version 6: introduce function pointer)
 *
 * Alex Wolf, July 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bag.h"

/**************** global variables ****************/

//local type
typedef struct bagData {
  void *data;		           // pointer to data for this item
  struct bagData *next;   // other elements in the bag(liked list)
} bagData_t;


//global type
typedef struct bag {
  struct bagData *first;   // first bag data element
  void (*destructor)(void *data);
} bag_t;

//function to create and return a new empty bag structure
bag_t *bag_new(void (*deleteFunction)(void*)){

	bag_t *bag = malloc(sizeof(bag_t));

	if(bag==NULL){
	fprintf(stderr, "bag sturct memory wasn't allocated properly");
	exit(1);
	}

	bag->destructor = NULL;
	bag->destructor =deleteFunction;// pointer to delte function
	bag->first=NULL;

	//exit if memory for bag wasn't allocated propelry
	return bag;
}

//insert data into the bag
void bag_insert(bag_t *bag, void *data){

	//create new bagData struct
	bagData_t *bagData = malloc(sizeof(bagData_t));
	void *Data = malloc(sizeof(void *));
	Data= data;
	bagData->data=NULL;
	bagData->next=NULL;
	bagData->data=Data;	//tag data


	if(bagData==NULL){
		fprintf(stderr, "bagData struct memory wasn't allocated properly");
		exit(1);
	}

	bagData_t *firstData= bag->first;

	//if there is no first element insert it to head of the bag
	if(firstData==NULL){
		bag->first=bagData;
	}

	//data is already in there
	else{
		bagData_t *dataElement= firstData;

		//looped through SLL of bag Data
		while(dataElement->next!=NULL){

			dataElement= dataElement->next; //continue through SLL of bagData			
		}

		//reached end of SLL, append data to end of list
		dataElement->next= bagData; 
	}
}

//extract last element in the SLL from the bag
//retun null if empty
void *bag_extract(bag_t *bag){

	bagData_t *bagData= bag->first;
	bagData_t *priorData;
	int count=0;

	//loop through SLL of bag Data until reached end
	while(bagData!=NULL){

		// //reached end of SLL
		if(bagData->next==NULL){

			void *returnedData= bagData->data;	//get the data to return

			//there was only one element in the bag
			if(priorData->next==NULL){
				bag->first=NULL;	
			}

			//there were mutiple elements
			else{
				//printf("######## was  priorData\n");
				priorData->next=NULL;	//remove from bag
			}
			printf("Final count of bag nodes left in bag is %i\n", count);

			return returnedData;
		}

		priorData=bagData; //keep trrack of previous Data
		bagData= bagData->next; //continue through SLL of bagData
		count+=1;	
	}

	//return null if bag empty
	return NULL;
}

//delete bag struct and bagData structs data within counters
void bag_delete(bag_t *bag){

	//get first counterdata
	bagData_t *bagData= bag->first;

	int countNum=1;

	//loop through all bag data
	while(bagData!=NULL){


		//delete void * data with function pointer
		(bag->destructor)(bagData->data);

		if((bagData->data)!=NULL){
			printf("Void * data in bagData deleted properly!\n");
		}


		bagData_t *bagDataToFree= bagData;

		bagData= bagData->next;

		//free the bagData struct
		free(bagDataToFree);


		if(bagDataToFree==NULL){
			fprintf(stderr, "ERROR: Couldn't free bagData properly\n");
			exit (2);
		}
		else{
			printf("Bag number %i deleted propely\n", countNum);
		}

		countNum+=1;
	}

	free(bag);

	if(bag==NULL){
			fprintf(stderr, "ERROR: Couldn't free bag properly\n\n");
			exit (2);
		}
		else{
			printf("bag struct deleted propely\n\n");
		}
}









