/* 
 * set.h - header file for bag data struct
 *
 * Alex Wolf, July 2016
 */
#include <stdbool.h>
 
#ifndef __SET_H
#define __SET_H

//define gobal structs
typedef struct set set_t;  // opaque to users of the module

/**************** functions ****************/
set_t *set_new(void (*deleteFunction)(void*));
void *set_find(set_t *set, char *key);
bool set_insert(set_t *set, char *key, void *data);
void set_delete(set_t *set);



#endif 