
/* 
 * index - a set of functions for creating, saving, loading the index
 *
 * Alex Wolf, Summer 2016
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "../lib//hashtable/hashtable.h"
#include "../lib/counters/counters.h"
#include "../lib/memory/memory.h"

#include "file.h"
#include "word.h"

//global structs
typedef struct hashtable index_t;



//create new index struct
index_t* index_new(const int indexSize);

//update index with docID
int index_document(int docID, char *pageDirectory, index_t *index);

//deelet index struct and data within
void index_delete(index_t *index);

//save the data in the indexto a file
bool index_save(FILE *fp, index_t *index);

//insurt countertrs struct into the hashtable with a give key
bool index_insert(index_t *index, char *word, void *counters);

//load data into index
index_t *loadIndex(char *oldFileName, index_t *index);

//finda counters with a given key
counters_t *index_find(index_t *index, char *word);

//stop function when a new line is read for file reading
int stopfunc(int c);

//split a char* to a char** with a delimeter
char** str_split(char* a_str, const char a_delim);

//helper function for str_split
char *strdup (const char *s);


