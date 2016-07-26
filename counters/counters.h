/* 
 * counters.h - header file for counters data struct
 *
 * Alex Wolf, July 2016
 */
#ifndef __COUNTERS_H
#define __COUNTERS_H


//define gobal structs
typedef struct counters counters_t;  // opaque to users of the module

//functions
counters_t *counters_new(void);
void counters_add(counters_t *ctrs, int key);
int counters_get(counters_t *ctrs, int key);
void counters_delete(counters_t *ctrs);

#endif