/* 
 * hashtable.c - simple hashtable data structure
 *
 * Alex Wolf, April 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../set/set.h"
#include "hashtable.h"


 /* local types*/
typedef struct setData {
  void *data;	// pointer to data for this item
  char *key;	// pointer to data for this item
  struct setData *next;   // other elements in the set(linked list)
} setData_t;

typedef struct set {
  struct setData *first;   // first bag data element
  void (*destructor)(void *data);
} set_t;

/***global type***/

//array of hastableData
typedef struct hashtable {
  int numSlots;
  set_t **slots; 
} hashtable_t;

//create a new hastable struct
hashtable_t *hashtable_new(const int num_slots){

	hashtable_t *hashtable = malloc(sizeof(hashtable_t));
	hashtable->numSlots=num_slots;

	hashtable->slots = malloc(num_slots * sizeof(set_t *));


	//check if hastable made properly
	if(hashtable==NULL){
		fprintf(stderr, "ERROR: Hashtable struct memory wasn't allocated properly\n");
		exit(1);
	}

	return hashtable;
}

//find a key in the hashtable
void *hashtable_find(hashtable_t *ht, char *key){

	//get the hash key
	int hash= (int) JenkinsHash(key, sizeof(ht->numSlots));

	set_t **slots= ht->slots;

	//get the set
	set_t *set= slots[hash];

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

	return NULL;
}

//insert data into hashtable
//true if data inserted
//false if data found
bool hashtable_insert(hashtable_t *ht, char *skey, void *sdata){

	//get the hash key
	int hash= (int) JenkinsHash(skey, sizeof(ht->numSlots));

	set_t **slots= ht->slots;
	setData_t *prevData;

	//check set for hash index if it doesn't exist
	if(slots[hash]==NULL){
		slots[hash] = set_new(free);

		//init set data and append to set
		setData_t *newSetData = malloc(sizeof(setData_t));
		newSetData->data= sdata;
		newSetData->key= skey;
		//printf("%s 1\n", newSetData->key);
		//make sure mem allocated properly

		slots[hash]->first=newSetData;
		return true;
	}

	//get the head setData
	setData_t *setData= slots[hash]->first;

	//loop through all data
	while(setData!=NULL){
		//printf("%s 2\n", setData->key);
		
		//set was found return false
		if(setData->key==skey){
			return false;
		}
		prevData=setData;
		setData=setData->next;
	}

	setData = malloc(sizeof(setData_t));
	setData->key=skey;
	setData->data=sdata;

	//link it to prev data
	prevData->next=setData;
	return true; //return true for inserting data
}


// JenkinsHash function
unsigned long JenkinsHash(const char *str, unsigned long mod){
    if (str == NULL)
      return 0;

    size_t len = strlen(str);
    unsigned long hash, i;

    for (hash = i = 0; i < len; ++i)
    {
        hash += str[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % mod;
}

//delete struct hashtable and all data in it including sets
//printing results for testing
void hashtable_delete(hashtable_t *ht){

	int hashNum= ht->numSlots;
	set_t **slots= ht->slots;

	int i;

	//loop through all sets
	for(i=0; i>hashNum; i++){

		set_delete(slots[hashNum]);	//seletes set including void data in it with function pointer
	}

	//delete the hashtable
	free(ht);

	if(ht==NULL){

		fprintf(stderr, "ERROR: Couldn't delete hashtable struct properly\n\n");
			exit (2);
		}

	else{
		printf("Hashtable struct deleted propely!!!\n");
	}
	
}







