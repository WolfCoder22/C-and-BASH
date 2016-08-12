 /*
 * set.h - header file for simple set module
 *
 * David Kotz, April 2016
 * updated by Xia Zhou, July 2016
 */

 #ifndef __SET_H
 #define __SET_H

 #include <stdbool.h>

 /**************** global types ****************/
 typedef struct set set_t; // opaque to users of the module

 /**************** functions ****************/

 /* Create a new (empty) set;
 * provide a function that will print an item,
 * and a function that will delete an item.
 * return NULL if error.
 */
 set_t *set_new( void (*itemprint)(FILE *fp, char *key, void *data),
 void (*itemdelete)(void *data) );

 /* Insert item into the given set; NULL set is ignored.
 * The item is described by a key and some data.
 * Return false if key already exists, set is NULL, or error occurs;
 * return true if new item was inserted.
 * The key string is copied for use by the set.
 */
 bool set_insert(set_t *set, char *key, void *data);

 /* Return the data associated with the given key;
 * return NULL if set is NULL or if key is not found.
 */
 void *set_find(set_t *set, char *key);

 /* Print the whole set. */
 void set_print(FILE *fp, set_t *set);

 /* Delete the whole set. */
 void set_delete(set_t *set);

 /* Iterate over all items in set (in undefined order):
 * call itemfunc for each item, passing (arg, key, data).
 */
void set_iterate(set_t *set,
		  void (*itemfunc)(void *arg, const char *key, void *data),
		  void *arg);


 #endif // __SET_H
