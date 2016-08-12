/*
* bag.h - header file for Lab3 'bag' module
*
* A ‘bag’ is a collection of indistinguishable items; it starts empty,
* grows as the caller adds one item at a time, and shrinks as the caller
* extracts one item at a time. It could be empty, or could contain hundreds
* of items. Since items are indistinguishable, the module is free to return
* any item from the bag.
*
* David Kotz, April 2016
*/

#ifndef __BAG_H
#define __BAG_H
/**************** global types ****************/
typedef struct bag bag_t; // opaque to users of the module

/**************** functions ****************/

/* Create a new (empty) bag;
* provide a function that will print an item,
* and a function that will delete an item.
* return NULL if error.
*/
bag_t *bag_new( void (*itemprint)(FILE *fp, void *data),
void (*itemdelete)(void *data) );

/* Add a new item into the given bag; NULL bag is ignored.
*/
void bag_insert(bag_t *bag, void *data);

/* Return any data item from the bag, or NULL if empty.
*/
void *bag_extract(bag_t *bag);

/* Print the whole bag. */
void bag_print(FILE *fp, bag_t *bag);

/* Delete the whole bag. */
void bag_delete(bag_t *bag);

/* Iterate over all items in bag (in undefined order):
 * call itemfunc for each item, passing (arg, key, data).
 */
void bag_iterate(bag_t *bag, void (*itemfunc)(void *data));

#endif // __BAG_H
