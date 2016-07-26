/* 
 * hashtable.h - header file for counters data struct
 *
 * Alex Wolf, July 2016
 */
 #include <stdbool.h>

#ifndef __HASHTABLE_H
#define __HASHTABLE_H


//define gobal structs
typedef struct hashtable hashtable_t;  // opaque to users of the module

//functions
hashtable_t *hashtable_new(const int num_slots);
void *hashtable_find(hashtable_t *ht, char *key);
bool hashtable_insert(hashtable_t *ht, char *key, void *data);
void hashtable_delete(hashtable_t *ht);
unsigned long JenkinsHash(const char *str, unsigned long mod);

#endif