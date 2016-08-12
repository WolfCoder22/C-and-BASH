 /*
 * set.c - simple set module
 *
 * David Kotz, April 2016
 * updated by Xia Zhou, July 2016
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <string.h>
 #include "set.h"

 /**************** file-local global variables ****************/
 // track malloc and free across *all* sets.
 static int nmalloc = 0; // number of successful malloc calls
 static int nfree = 0; // number of free calls
 static int nfreenull = 0; // number of free(NULL) calls

 /**************** local types ****************/
 typedef struct setnode {
 	char *key; // search key for this item
 	void *data; // pointer to data for this item
 	struct setnode *next; // pointer to next item in set
 } setnode_t;

 /**************** global types ****************/
 typedef struct set {
 	struct setnode *head; // head of the set
 	void (*itemprint)(FILE *fp, char *key, void *data); // function to print an item
 	void (*itemdelete)(void *data); // function to delete an item
 } set_t;

 /**************** global functions ****************/
 /* that is, visible outside this file */
 /* see set.h for comments about exported functions */

 /**************** local functions ****************/
 /* not visible outside this file */
 static setnode_t *setnode_new(char *key, void *data);
 static void *count_malloc(size_t size);
 static void count_free(void *ptr);
 #ifdef TESTING
	static void count_report(FILE *fp);
 #endif

 /**************** set_new() ****************/
 set_t *
 set_new( void (*itemprint)(FILE *fp, char *key, void *data) ,
  void (*itemdelete)(void *data) )
 {
 	set_t *set = count_malloc(sizeof(set_t));

 	if (set == NULL) {
 		return NULL; // error allocating set
 	} else {
 		// initialize contents of set structure
 		set->head = NULL;
 		set->itemprint = itemprint;
		set->itemdelete = itemdelete;
 		return set;
 	}
 }

 /**************** set_insert() ****************/
 bool
 set_insert(set_t *set, char *key, void *data)
 {
 	bool inserted = false; // function result
 	bool exist = false; // whether the key exists

 	if (set == NULL)
 		return false; // bad set

 	// check if the key already exists in the set
 	for (setnode_t *node = set->head; node != NULL; node = node->next)
 		if (strcmp(key, node->key) == 0) {
 			exist = true;
		 	break;
		}

 		// insert new node at the head of set if it's a new key
 		if (!exist) {
 			setnode_t *new = setnode_new(key, data);
 		if (new != NULL) {
 			new->next = set->head;
 			set->head = new;
 			inserted = true;
 		}
 	}

	#ifdef TESTING
		fprintf(stdout, "After set_insert(%s): ", key);
		count_report(stdout);
 	#endif

	return inserted;
 }


/**************** setnode_new ****************/
/* Allocate and initialize a setnode */
static setnode_t * // not visible outside this file
setnode_new(char *key, void *data)
{
 	setnode_t *node = count_malloc(sizeof(setnode_t));

	if (node == NULL) {
		// error allocating memory for node; return error
		return NULL;
	} else {
 		node->key = count_malloc(strlen(key)+1);
		if (node->key == NULL) {
			// error allocating memory for key;
			// cleanup and return error
			count_free(node);
			return NULL;
 		} else {
 			strcpy(node->key, key);
 			node->data = data;
 			node->next = NULL;
 			return node;
 		}
 	}
}

/**************** set_find() ****************/
void *
set_find(set_t *set, char *key)
{
 	if (set == NULL)
		return NULL; // bad set
	else {
		// scan the set
		for (setnode_t *node = set->head; node != NULL; node = node->next)
			if (strcmp(key, node->key) == 0)
				return node->data; // found! return the node's data

			return NULL; // not found!
	}
}

 /**************** set_print() ****************/
void
set_print(FILE *fp, set_t *set)
{
 	if (set == NULL) {
 		fputs("(null)", fp);
		return; // bad set
	} else {
		// scan the set
		fputc('[', fp);
		for (setnode_t *node = set->head; node != NULL; node = node->next) {
			// print the current node
			if (set->itemprint != NULL) // print the node
				(*set->itemprint)(fp, node->key, node->data);
				fputs(", ", fp); // print a comma-space
			}
		fputc(']', fp);
	}
	return;
 }

/**************** set_delete() ****************/
void
set_delete(set_t *set)
 {
	if (set == NULL)
		return; // bad set
	else {
		setnode_t *node = set->head;
		while (node != NULL) {
			if (set->itemdelete != NULL) // if necesssary...
				(*set->itemdelete)(node->data); // delete node's data
				setnode_t *next = node->next; // remember what's next
				count_free(node->key); // delete current node's key
 				count_free(node); // delete current node
				node = next; // move on to next
			}

			// delete the overall structure
			count_free(set);
		}

	#ifdef TESTING
		fprintf(stdout, "End of set_delete: ");
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

/* Iterate over all items in set (in undefined order):
 * call itemfunc for each item, passing (arg, key, data).
 */
void set_iterate(set_t *set,
		  void (*itemfunc)(void *arg, const char *key, void *data),
		  void *arg){

	//loop through all set nodes and get desired data
	for (setnode_t *node = set->head; node != NULL; node = node->next){
			
		char *key= node->key;
		void *data= node->data;
		itemfunc(arg, key, data);

	}

}

#ifdef TESTING
/**************** count_report() ****************/
/* report the current malloc/free counts
 */
static void
count_report(FILE *fp)
{
	fprintf(fp, "set: %d malloc, %d free, %d free(NULL), %d net\n",
	 nmalloc, nfree, nfreenull, nmalloc - nfree - nfreenull);
}
#endif



