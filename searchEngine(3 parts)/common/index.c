/* 
 * index - a set of functions for creating, saving, loading the index
 * 
 * See index.h for interface descriptions.
 * NOTE: NEED TO COMPLETE ITEMPRINT AND ITEMDELETE
 * Alex Wolf, Summer 2016
 */
#include <string.h>
#include "index.h"

//local functions
static void index_itemdelete(void *data);
static void wordScan(void *fp, const char *word, void *counters);
static void countersScan(void *fp, const int id, int count);

//create new index struct
index_t* index_new(const int indexSize){

	index_t *index = hashtable_new(indexSize, NULL, index_itemdelete);
	assertp(index, "Index was not succesfully created");

	return index;
}

//update index with docID
int index_document(int docID, char *pageDirectory, index_t *index){

	char filename[100];
	//get proper filename
    sprintf(filename, "%s/%d", pageDirectory, docID);

    //create and write to file
    FILE *fp= fopen(filename, "r");

    //make sure file pointer works properly and exists
    //returns 1 if doesn't exists or can't be opened
    if(fp==NULL){
      return 1;
    }

    else{

    	//ignore first two lines
    	char *line1=readuntil(fp, stopfunc);
    	char *line2=readuntil(fp, stopfunc);

   		
   		//Allocates the memory for the html
    	char *html= readfile(fp);

    	fclose(fp);

    	//free strings for heap
    	free(line1);
    	free(line2);

	    int pos=0;
	    char *word;

	     //go through all words in document
	     while((pos= GetNextWord(html, pos, &word)) != -1){
	     	
	      		//normalize the word
	      		NormalizeWord(word);

	      		//see if counter exists in ht
	      		counters_t *counters=index_find(index, word);

	      		if(counters==NULL){

	      			//init new counter with key=docID
	      			counters= counters_new();
	      			counters_add(counters, docID);

	      			//insert into the Hashtable
	      			index_insert(index, word, counters);
	      		}
	      		//counter already exists for that word increment it for that docID
	      		else{
	      			counters_add(counters, docID);
	      		}
	      		free(word);
	    	}	
	    	//return 0 signalling a sucess
	    	free(html);
	    	return 0;
	    }
}

//return the counter returned from the index by a key
counters_t *index_find(index_t *index, char *word){

	return hashtable_find(index, word);
}

//inset counters data into the hashtable
bool index_insert(index_t *index, char *word,void *counters){

	if(hashtable_insert(index, word, counters)){
		return true;
	}
	return false;
	
}

//save the data in the index to a file
bool index_save(FILE *fp, index_t *index){
	if (index == NULL) {
		printf("ERROR: Index has NULL pointer\n");
		return false;
	}
	else if (fp == NULL) {
		printf("File pointer wasn'it intilaized properlu\n");
		return false;
	}
	else {

		//deal with data in inex with word scan function
		hashtable_iterate(index, wordScan, fp);
		return true;
	}
}

//load data into index
index_t *loadIndex(char *oldFileName, index_t *index){

	//open file
	FILE *fp= fopen(oldFileName, "r");
	int totalLines= lines_in_file(fp);
	rewind(fp);
	int i;

	//for all line's in file
	for(i=0; i<totalLines; i++){
		char *line=readline(fp);
		
		//get array from line
		char **words= str_split(line, ' ');
		free(line);

		//vars for char ** parsing
		int t=0;
		int charArrazySize=0;

		//get number of words in line
		while(words[t]!=NULL){
			charArrazySize++;
			t++;
		} 

		char *key=words[0];
		int charIndex;

		//insert counters struct into index for each key
		counters_t *counters= counters_new();
		index_insert(index, key, counters);

		free(key);

		//for the rest of the words in the file put in counters and insert
		for(charIndex=1; charIndex<charArrazySize; charIndex=charIndex+2){

			//get the docid and count for that entry
			int docID= atoi(words[charIndex]);
			int count= atoi(words[charIndex+1]);

			counters_set(counters, docID, count);

			free(words[charIndex]);
			free(words[charIndex+1]);
		}

		free(words);
	}
	fclose(fp);
    return index;
}


//stop function for file reading
int stopfunc(int c){

    if(c == '\n'){
    	return true;
    }
    return false;
} 

//function to deltete a hashtable and malloced counters struct in it
static void index_itemdelete(void *data){
  if(data==NULL){
    return;
  }
  counters_delete(data);
}

//delete index struct and data within
void index_delete(index_t *index){
	hashtable_delete(index);
}


//scan for a word and the data associitted with it
static void wordScan(void *fp, const char *word, void *counters){
	if (fp == NULL) {
		printf("ERROR:File Pointer is null\n");
		return;
	}
	else if (word == NULL) {
		printf("ERROR: Counters Key NULL\n");
		return;
	}
	else if (counters == NULL) {
		printf("ERROR: Counters Object NULL\n");
		return;
	}
	else {
		//only write words that are at least 3 characters long
		if(strlen(word)>3){

			//convert const char *to char *
			char *wordOnHeap = malloc(strlen(word)+1);
			if (wordOnHeap) {
   				strcpy(wordOnHeap, word);
			}

			//normalize the word
			word=NormalizeWord(wordOnHeap);

			// write word to file
			fprintf(fp, "%s ", word);

			free(wordOnHeap);

			//write word data to file
			counters_iterate(counters, countersScan, fp);
			fprintf(fp, "\n");
		}
	}
}

//scan for data for each word and print to file
static void countersScan(void *fp, const int id, int count){
	// id count id count id count...
	fprintf(fp, "%d %d ", id, count);
}

/** from stackoverflow***
	splits a string by the give char into an array
	edited by Alaex WOlf    */
char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);

            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }
    return result;
}

/***from stackoverflow***
*****Needed for function above****/
char *strdup (const char *s) {
    char *d = malloc (strlen (s) + 1);   // Space for length plus nul
    if (d == NULL) return NULL;          // No memory
    strcpy (d,s);                        // Copy the characters
    return d;                            // Return the new string
}

