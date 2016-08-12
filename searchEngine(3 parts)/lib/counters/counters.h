/*
 * counters.h - header file for simple counters module
 *
 * David Kotz, April 2016
 */

 #ifndef __COUNTERS_H
 #define __COUNTERS_H

 /**************** global types ****************/
 typedef struct counters counters_t; // opaque to users of the module

 /**************** functions ****************/

 /* Create a new (empty) counter structure;
 * return NULL if error.
 */
 counters_t *counters_new(void);

 /* Increment the counter indicated by key.
 * If the key does not yet exist, create a counter for it and initialize to 1.
 * NULL counters is ignored.
 */
 void counters_add(counters_t *ctrs, int key);

 /* Return current value of counter associated with the given key;
 * return 0 if counters is NULL or if key is not found.
 */
 int counters_get(counters_t *ctrs, int key);

 /* Print all counters. */
 void counters_print(FILE *fp, counters_t *ctrs);

 /* Delete the whole counters. */
 void counters_delete(counters_t *ctrs);

//
 void counters_set(counters_t *ctrs, int key, int count);

/* Iterate over all counters in the set (in undefined order):
 * call itemfunc for each item, with (arg, key, count).
 */
void counters_iterate(counters_t *ctrs,
		      void (*itemfunc)(void *arg, const int key, int count),
		      void *arg);


 #endif // __COUNTERS_H
 
