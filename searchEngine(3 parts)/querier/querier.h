/* =========================================================================
 * File: querier.h
 *
 * Project name: CS50 Tiny Search Engine
 * Component name: Querier
 *
 * Author: Alex Wolf, August 2016
 * 
 *
 * ========================================================================= */
#ifndef QUERIER_H
#define QUERIER_H
#define _GNU_SOURCE

#include <stdlib.h> 
#include <stdio.h>                        
#include <stdbool.h>
#include <dirent.h>
#include <ctype.h>
#include <string.h>
#include "../common/index.h"
#include "../common/file.h"
#include "../common/word.h"
 #include "../lib/bag/bag.h"

//global vars
#define BUFFER_SIZE 2000

 /**************** global types ****************/
 typedef struct aResult aResult_t;
 
//local functions

//parse the params and make sure they are correct
void parseParams(int argc, char *argv[]);

//get number of results
int getResultCount(counters_t *results);

//compare two score of a reult
static int cmpfunction(const void * a, const void * b);

//test if a file exists for dir counting purposes
bool fileExists(int docID);

//determine if a line in the query input is valid
void validateQuery(char *line);

//helper function to get number of results
static void resultCount_helper(void *arg, int key, int count);

//parse the stdin for a query
void parseQuery();

//get the stdin for the query
char * getStdin();

//score all the docs in the crawler doc directory 
void runQuery(char **words, int wordIndex);

//match two counter objects by interating through counters A
void match_intersect(counters_t *A, counters_t *B);

//loop through one counter and figure out which docunents have the same
static void intersect_helper(void *arg, int key, int count);

//matchs a or
static void match_union(counters_t *A, counters_t *B);

//print score and url of found page
void printURLscore(int docID, int score);

//helper function for oring
static void union_helper(void *arg, int key, int count);

//see which int is smaller for AND queries
static int min(int a, int b);

//get number of results
aResult_t **initResults(counters_t *resultS, aResult_t **results);

//helper function to get number of results
static void initARusult(void *arg, int key, int count);

//print the results of the query
void printResults(counters_t *resultCounters);

//score all the arrays in the bag
void scoreQuery(char ***andArrays, int arraySize, int *counts);

//get the score of a single word array
counters_t *getAndScore(char **words, int wordCount);

//helper function to get a word array from a bag
void getWords(void *data);

//print the results of the query
void printResults(counters_t *results);

//create a new result item
aResult_t *aResult_new(int ID, int Score);

#endif 

