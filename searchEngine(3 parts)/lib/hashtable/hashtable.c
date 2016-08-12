/*
* hashtable.c - simple hashtable module
*
* David Kotz, April 2016
* updated by Xia Zhou, July 2016
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
#include "../set/set.h"
#include "jhash.h"

/**************** file-local global variables ****************/
// track malloc and free across *all* hashtables.
static int nmalloc = 0; // number of successful malloc calls
static int nfree = 0; // number of free calls
static int nfreenull = 0; // number of free(NULL) calls

/**************** local types ****************/

/**************** global types ****************/
typedef struct hashtable {
	int num_slots; // number of slots in the table
	set_t **table; // table[num_slots] of set_t*
	void (*itemprint)(FILE *fp, char *key, void *data); // function to print an item
	void (*itemdelete)(void *data); // function to delete an item
} hashtable_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see hashtable.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static void *count_malloc(size_t size);
static void count_free(void *ptr);
#ifdef TESTING
	static void count_report(FILE *fp);
#endif

/**************** hashtable_new() ****************/
hashtable_t *
hashtable_new( const int num_slots,
void (*itemprint)(FILE *fp, char *key, void *data) ,
void (*itemdelete)(void *data) )
{
	hashtable_t *ht = count_malloc(sizeof(hashtable_t));
	if (ht == NULL) {
		return NULL; // error allocating hashtable
	}

	// initialize contents of hashtable structure
	ht->num_slots = num_slots;
	ht->itemprint = itemprint;
	ht->itemdelete = itemdelete;
	ht->table = count_malloc(num_slots * sizeof(set_t *));
	if (ht->table == NULL) {
		count_free(ht); // error allocating table
		return NULL;
	}

	// initialize each table entry to be a set
	for (int slot = 0; slot < num_slots; slot++) {
		set_t *new = set_new(itemprint, itemdelete);
	if (new != NULL) {
		ht->table[slot] = new;
	}
	else {
		// malloc failure; unwind all we've created. [sigh]
		while (--slot >= 0)
			set_delete(ht->table[slot]);
			count_free(ht->table);
			count_free(ht);
			return NULL;
		}
	}

	return ht;
}

/**************** hashtable_insert() ****************/
bool
hashtable_insert(hashtable_t *ht, char *key, void *data)
{
	if (ht == NULL)
		return false; // bad hashtable

	int slot = JenkinsHash(key, ht->num_slots);

	bool inserted = set_insert(ht->table[slot], key, data);

#ifdef TESTING
		fprintf(stdout, "After hashtable_insert(%s): ", key);
		count_report(stdout);
	#endif

	return inserted;
}


/**************** hashtable_find() ****************/
void *
hashtable_find(hashtable_t *ht, char *key)
{
	if (ht == NULL)
		return NULL; // bad hashtable
	else {
		int slot = JenkinsHash(key, ht->num_slots);
		return set_find(ht->table[slot], key);
	}
}

/**************** hashtable_print() ****************/
void
hashtable_print(FILE *fp, hashtable_t *ht)
{
	if (ht == NULL) {
		fputs("(null)", fp); // bad hashtable
	} else {
		// print one line per slot
		for (int slot = 0; slot < ht->num_slots; slot++) {
		printf("%4d: ", slot);
			set_print(fp, ht->table[slot]);
			printf("\n");
		}
}
}

/**************** hashtable_delete() ****************/
void
hashtable_delete(hashtable_t *ht)
{
	if (ht == NULL)
		return; // bad hashtable
	else {
		// delete set in each slot
	for (int slot = 0; slot < ht->num_slots; slot++)
			set_delete(ht->table[slot]);
			// delete the table, and the overall struct
			count_free(ht->table);
			count_free(ht);
		}

	#ifdef TESTING
		fprintf(stdout, "End of hashtable_delete: ");
		count_report(stdout);
	#endif
}

/* Iterate over all items in hashtable (in undefined order):
 * call itemfunc for each item, with (arg, key, data).
 */
void hashtable_iterate(hashtable_t *ht, 
		       void (*itemfunc)(void *arg, const char *key,void *data),
		       void *arg){

	if (ht == NULL) {
		printf("ERROR: invalid Hastable for itemfunc\n");
		exit(3);
	} else {

		set_t **slots=ht->table;

		// loop through all sets in hashtable
		for (int slot = 0; slot < ht->num_slots; slot++) {
			
			set_t *slotRow= slots[slot];

			//call set interate
			set_iterate(slotRow, itemfunc, arg);
		}	
	}
}

/**************** count_malloc() ****************/
/* just like malloc() but track the number of successful allocations
*/
static void *
count_malloc(size_t size)
{
	void *ptr = malloc(size);
	if (ptr != NULL)
		nmalloc++;
		return ptr;
}

/**************** count_free() ****************/
/* just like free() but track the number of calls
*/
static void
count_free(void *ptr)
{
	if (ptr != NULL) {
		free(ptr);
		nfree++;
	} else {
		// it's an error to call free(NULL)!
		nfreenull++;
	}
}

#ifdef TESTING
/**************** count_report() ****************/
/* report the current malloc/free counts
*/
static void
count_report(FILE *fp)
{
	fprintf(fp, "hashtable: %d malloc, %d free, %d free(NULL), %d net\n",
	 nmalloc, nfree, nfreenull, nmalloc - nfree - nfreenull);
}
#endif
