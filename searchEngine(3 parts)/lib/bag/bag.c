/*
 * bag.c - implementation file for Lab3 'bag' module
 *
 * David Kotz, April 2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bag.h"

/**************** file-local global variables ****************/
// track malloc and free across *all* bags.
static int nmalloc = 0; // number of successful malloc calls
static int nfree = 0; // number of free calls
static int nfreenull = 0; // number of free(NULL) calls

/**************** local types ****************/
typedef struct bagnode {
	void *data; // pointer to data for this item
	struct bagnode *next; // link to next node
} bagnode_t;

/**************** global types ****************/
typedef struct bag {
	struct bagnode *head; // head of the list of items in bag
	void (*itemprint)(FILE *fp, void *data); // function to print an item
	void (*itemdelete)(void *data); // function to delete an item
} bag_t;

/**************** global functions ****************/
/* that is, visible outside this file */
/* see bag.h for comments about exported functions */

/**************** local functions ****************/
/* not visible outside this file */
static bagnode_t *bagnode_new(void *data);
static void *count_malloc(size_t size);
static void count_free(void *ptr);
#ifdef TESTING
static void count_report(FILE *fp);
#endif

/**************** bag_new() ****************/
bag_t *
bag_new( void (*itemprint)(FILE *fp, void *data) ,
	void (*itemdelete)(void *data) )
{
	bag_t *bag = count_malloc(sizeof(bag_t));
	if (bag == NULL) {
		return NULL; // error allocating bag
	} else {
	// initialize contents of bag structure
		bag->head = NULL;
		bag->itemprint = itemprint;
		bag->itemdelete = itemdelete;
		return bag;
	}
}

/**************** bag_insert() ****************/
void
bag_insert(bag_t *bag, void *data)
{
	if (bag != NULL) {
		// allocate a new node to be added to the list
		bagnode_t *new = bagnode_new(data);
		if (new != NULL) {
			// add it to the head of the list
			new->next = bag->head;
			bag->head = new;
		}
	}

#ifdef TESTING
	fprintf(stdout, "After bag_insert: ");
	count_report(stdout);
#endif
}

/**************** bagnode_new ****************/
/* Allocate and initialize a bagnode */
static bagnode_t * // not visible outside this file
bagnode_new(void *data)
{
bagnode_t *node = count_malloc(sizeof(bagnode_t));
	
	if (node == NULL) {
		// error allocating memory for node; return error
		return NULL;
		} else {
			node->data = data;
			node->next = NULL;
			return node;
		}
}

/**************** bag_extract() ****************/
void *
bag_extract(bag_t *bag)
{
	if (bag == NULL)
		return NULL; // bad bag
	else if (bag->head == NULL)
		return NULL; // bag is empty
	else {
		bagnode_t *out = bag->head; // the node to take out
		void *data = out->data; // the data to return
		bag->head = out->next; // hop over the node to remove
		count_free(out);
		return data;
	}
}

/**************** bag_print() ****************/
void
bag_print(FILE *fp, bag_t *bag)
{
	if (bag != NULL) {
		fputc('{', fp);
	for (bagnode_t *node = bag->head; node != NULL; node = node->next) {
		// print this node
		if (bag->itemprint != NULL) // print the node
			(*bag->itemprint)(fp, node->data);
			fputs(", ", fp); // separate with comma-space
		}
		fputc('}', fp);
	} else
		fputs("(null)", fp);
}

/* Iterate over all items in bag (in undefined order):
 * call itemfunc for each item, passing (arg, key, data).
 */
void bag_iterate(bag_t *bag, void (*itemfunc)(void *data)){

	//loop through all set nodes and get desired data
	for (bagnode_t *node = bag->head; node != NULL; node = node->next){
			
		void *data= node->data;
		itemfunc(data);
	}
}

/**************** bag_delete() ****************/
void
bag_delete(bag_t *bag)
{
	if (bag != NULL) {
		for (bagnode_t *node = bag->head; node != NULL; ) {
			if (bag->itemdelete != NULL) // if possible...
				(*bag->itemdelete)(node->data); // delete node's data
			bagnode_t *next = node->next; // remember what comes next
			count_free(node); // free the node
			node = next; // and move on to next
		}

		count_free(bag);
	}

#ifdef TESTING
	fprintf(stdout, "End of bag_delete: ");
	count_report(stdout);
#endif
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
	fprintf(fp, "bag: %d malloc, %d free, %d free(NULL), %d net\n",
	 malloc, nfree, nfreenull, nmalloc - nfree - nfreenull);
}
#endif
