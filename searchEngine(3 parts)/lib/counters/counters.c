 /*
 * counters.c - module to support a set of counters
 *
 * David Kotz, April 2016
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "counters.h"

 /**************** file-local global variables ****************/
 // track malloc and free across *all* counterss.
 static int nmalloc = 0; // number of successful malloc calls
 static int nfree = 0; // number of free calls
 static int nfreenull = 0; // number of free(NULL) calls

 /**************** local types ****************/
 typedef struct countersnode {
 int key; // search key for this counter
 int count; // value of this counter
 struct countersnode *next; // pointer to next counter in list
 } countersnode_t;

 /**************** global types ****************/
 typedef struct counters {
 struct countersnode *head; // head of the counters (UNSORTED list)
 } counters_t;

 /**************** global functions ****************/
 /* that is, visible outside this file */
 /* see counters.h for comments about exported functions */

 /**************** local functions ****************/
 /* not visible outside this file */
 static countersnode_t *countersnode_new(int key);
 static void *count_malloc(size_t size);
 static void count_free(void *ptr);
 #ifdef TESTING
 static void count_report(FILE *fp);
 #endif

 /**************** counters_new() ****************/
 counters_t *
 counters_new(void)
 {
 counters_t *ctrs = count_malloc(sizeof(counters_t));

 if (ctrs == NULL) {
 return NULL; // error allocating counters
 } else {
 // initialize contents of counters structure
 ctrs->head = NULL;
 return ctrs;
 }
 }

 /**************** counters_add() ****************/
 void
 counters_add(counters_t *ctrs, int key){

 	if (ctrs == NULL){
 		return; // bad counters
 	}

 	if (ctrs->head == NULL) {

 		// empty list: add new counter
 		ctrs->head = countersnode_new(key);
 	}
 	else { // look for the counter in the list

 		// it is an unsorted list, so scan until we find it, or reach the end
 		countersnode_t *prev = ctrs->head; // which we know not to be NULL

	 	for (countersnode_t *node = prev; node != NULL; node = prev->next) {

	 		if (key == node->key) {
	 			// counter exists: increment it
	 			node->count++;
	 			goto done;
	 		}
	 		prev = node;
	 	}

 		// not on the list; insert new counter node at end of list
 		prev->next = countersnode_new(key);
 	}
 	done:;
 	#ifdef TESTING
 	fprintf(stdout, "After counters_add(%d): ", key);
 	count_report(stdout);
 #endif
 }


 /**************** countersnode_new ****************/
 /* Allocate and initialize a countersnode */
 static countersnode_t * // not visible outside this file
 countersnode_new(int key)
 {
 countersnode_t *node = count_malloc(sizeof(countersnode_t));

 if (node == NULL) {
 // error allocating memory for node; return error
 return NULL;
 } else {
 node->key = key;
 node->count = 1;
 node->next = NULL;
 return node;
 }
 }


 /**************** counters_get() ****************/
 int
 counters_get(counters_t *ctrs, int key)
 {
 if (ctrs == NULL)
 return 0; // bad counters
 else {
 // scan the counters
 for (countersnode_t *node = ctrs->head; node != NULL; node = node->next)
 if (key == node->key)
 return node->count; // found! return the node's count

 return 0; // not found!
 }
 }

 /**************** counters_print() ****************/
 void
 counters_print(FILE *fp, counters_t *ctrs)
 {
 if (ctrs == NULL) {
 fputs("(null)", fp);
 return; // bad counters
 } else {
 // scan the counters
 fputc('[', fp);
 for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) {
 // print the current node
 fprintf(fp, "%d=%d, ", node->key, node->count);
 }
 fputc(']', fp);
 }
 return;
 }

 /**************** counters_delete() ****************/
 void
 counters_delete(counters_t *ctrs)
 {
 if (ctrs == NULL)
 return; // bad counters
 else {
 countersnode_t *node = ctrs->head;
 while (node != NULL) {
 countersnode_t *next = node->next; // remember what's next
 count_free(node); // delete the current node
 node = next; // move on to next
 }
 // delete the overall structure
 count_free(ctrs);
 }
 #ifdef TESTING
 fprintf(stdout, "End of counters_delete: ");
 count_report(stdout);
 #endif
 }

/* Set value of the counter indicated by key.
 * If the key does not yet exist, create a counter for it
 * and initialize its counter value to 'count'.
 * NULL counters is ignored.
 */
void counters_set(counters_t *ctrs, int key, int count){

	
 	if (ctrs == NULL){
 		return; // bad counters
 	}

 	if (ctrs->head == NULL) {

 		// empty list: add new counter and set count
 		ctrs->head = countersnode_new(key);
 		ctrs->head->count=count;
 	}
 	else { // look for the counter in the list

 		// it is an unsorted list, so scan until we find it, or reach the end
 		countersnode_t *prev = ctrs->head; // which we know not to be NULL

	 	for (countersnode_t *node = prev; node != NULL; node = prev->next) {

	 		if (key == node->key) {
	 			// counter exists: update count
	 			node->count=count;
	 			return;
	 		}
	 		prev = node;
	 	}

 		// not on the list; insert new counter node at end of list
 		prev->next = countersnode_new(key);
 		prev->next->count=count;	//update count
 		return;
 	}	
	
}




/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 */
void counters_iterate(counters_t *ctrs,
		      void (*itemfunc)(void *arg, const int key, int count),
		      void *arg){
	 //ctrs is null
	 if (ctrs == NULL) {
	 	printf("ERROR: Counters was null");
	 	exit(3);
 	} 
 	else {

	 	//loop through all counters in set
		for (countersnode_t *node = ctrs->head; node != NULL; node = node->next) {

			// call intemfunc on counter node
			int key=node->key;
			int count =node->count;
			itemfunc(arg, key, count);
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
 fprintf(fp, "counters: %d malloc, %d free, %d free(NULL), %d net\n",
 nmalloc, nfree, nfreenull, nmalloc - nfree - nfreenull);
 }
 
 #endif

