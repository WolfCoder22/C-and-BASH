/* 
 * bag.h - header file for bag data struct
 *
 * Alex Wolf, July 2016
 */

#ifndef __BAG_H
#define __BAG_H

//define gobal structs
typedef struct bag bag_t;  // opaque to users of the module

/**************** functions ****************/
bag_t *bag_new(void (*deleteFunction)(void*));
void bag_insert(bag_t *bag, void *data);
void *bag_extract(bag_t *bag);
void bag_delete(bag_t *bag);

#endif 