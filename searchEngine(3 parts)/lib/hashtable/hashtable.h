/*
* hashtable.h - header file for simple hashtable module
*
* David Kotz, April 2016
* updated by Xia Zhou, July 2016
*/

#ifndef __HASHTABLE_H
#define __HASHTABLE_H

#include <stdbool.h>

/**************** global types ****************/
typedef struct hashtable hashtable_t; // opaque to users of the module

/**************** functions ****************/

/* Create a new (empty) hashtable;
* provide a function that will print an item,
* and a function that will delete an item.
* return NULL if error.
*/
hashtable_t *hashtable_new( const int num_slots,
void (*itemprint)(FILE *fp, char *key, void *data),
void (*itemdelete)(void *data) );

/* Insert item into the given hashtable; NULL hashtable is ignored.
* The item is described by a key and some data.
* Return false if key already exists, hashtable is NULL, or error occurs;
* return true if new item was inserted.
* The key string is copied for use by the hashtable.
*/
bool hashtable_insert(hashtable_t *ht, char *key, void *data);

/* Return the data associated with the given key;
* return NULL if hashtable is NULL or if key is not found.
*/
void *hashtable_find(hashtable_t *ht, char *key);

/* Print the whole hashtable. */
void hashtable_print(FILE *fp, hashtable_t *ht);

/* Delete the whole hashtable. */
void hashtable_delete(hashtable_t *ht);

/* Iterate over all items in hashtable (in undefined order):
 * call itemfunc for each item, with (arg, key, data).
 */
void hashtable_iterate(hashtable_t *ht, 
		       void (*itemfunc)(void *arg, const char *key,void *data),
		       void *arg);

#endif // __HASHTABLE_H
