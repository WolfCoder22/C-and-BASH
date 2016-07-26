/* 
 * set.c - simple set data structure
 *
 * Alex Wolf, April 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "set.h"

/*local type */
typedef struct setData {
  void *data;	// pointer to data for this item
  char *key;	// pointer to data for this item
  struct setData *next;   // other elements in the set(linked list)
} setData_t;

//global type
typedef struct set {
  struct setData *first;   // first bag data element
  void (*destructor)(void *data);
} set_t;

//make a new set struct with no data and return it
set_t *set_new(void (*deleteFunction)(void*)){

	set_t *set = malloc(sizeof(set_t));
	set->first=NULL;

	if(set==NULL){
		fprintf(stderr, "set sturct memory wasn't allocated properly");
		exit(1);
	}
	set->destructor = deleteFunction;// pointer to delete function

	return set;
}

//insert a data into the set with the given key
bool set_insert(set_t *set, char *key, void *data){

	//get the head setData
	setData_t *setData= set->first;
	setData_t *prevSetData;

	//there are items in the set
	if(setData!=NULL){

		//loop through all data
		while(setData!=NULL){
			char *setELemKey= setData->key;

			//the key was found. return false
			if(strcmp(setELemKey, key)==0){
				return false;
			}
			//move to next data item
			else{
				prevSetData= setData;
				setData=setData->next;
			}
		}
		prevSetData->next= setData;
	}

	//init set data
	setData_t *newSetData = malloc(sizeof(setData_t));
	newSetData->data=data;
	newSetData->key= key;
	newSetData->next=NULL;

	//make sure mem allocated properly
	if(newSetData==NULL){
		fprintf(stderr, "setData sturct memory wasn't allocated properly");
		exit(1);
	}

	//if the set is empty
	if(prevSetData==NULL){
		set->first=newSetData;
	}
	//set not empty append to end
	else{
		prevSetData->next= newSetData;
	}

	return true;
}

//find a data point based off existing key
//return false(0) if key not found
void *set_find(set_t *set, char *key){

	//get the head setData
	setData_t *setData= set->first;

	//loop through all data
	while(setData!=NULL){

		char *setKey= setData->key;

		//key was found, return data
		if(strcmp(setKey, key)==0){
			return setData->data;
		}

		setData= setData->next;
	}

	//key wasn't found return false
	return false;
}

//delete struct set and all struct data in it
//printing results for testing
void set_delete(set_t *set){

	setData_t *dataSet= set->first;

	int countNum=1;

	//loop through all counters
	while(dataSet!=NULL){
		char *key= dataSet->key;

		setData_t *dataSetToFree= dataSet;
		dataSet= dataSet->next;

		//free void data in set with function pointer
		(set->destructor)(dataSetToFree->data);

		if(dataSetToFree->data!=NULL){
			printf("Void data from struct deleted properly!\n");
		}
	
		free(dataSetToFree);

		if(dataSetToFree==NULL){
			fprintf(stderr, "ERROR: Couldn't setData struct properly!\n");
			exit (2);
		}
		else{

			printf("setData struct number %i with key, %s, deleted propely\n",countNum, key);
		}

		countNum+=1;
	}

	//finally free the whole set
	free(set);
	if(set==NULL){
			fprintf(stderr, "ERROR: Couldn't set struct properly\n\n");
			exit (2);
		}
		else{
			printf("set struct deleted propely\n\n");
		}

}

