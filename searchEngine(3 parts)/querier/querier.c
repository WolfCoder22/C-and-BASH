/* 
 * querier - query and rank urls for the search engine
 *
 * Alex wolf, August 2016
 */

//include header file data
#include "querier.h"

//global vars
char *pageDirectory;
char *indexFileName;
char stdInBuff[BUFFER_SIZE];
char** words;
index_t *Index;
int numResults=0;
int initResultCount=0;

/**************** global types ****************/
typedef struct aResult{
	int docID;
	int score;
} aResult_t;

//main function for indexer
int main(int argc, char *argv[]){

	printf("\n");

  	parseParams(argc, argv);  //parse commands 
  	Index=index_new(1000);  //init data structs for crawl
  
  	//load indexFile data into the index struct
  	Index=loadIndex(indexFileName, Index);

  	//parse query from stdin
  	parseQuery();

	//free the index
	index_delete(Index);
}

//parse query from stdin
void parseQuery(){

	//get stdin
	char *stanIn= getStdin();

	//init vars
	char queries[strlen(stanIn)+1];
	strcpy(queries, stanIn);
	char *sep = "\n";
	char *line, *brkt;

	//loop through all lines
	for (line = strtok_r(queries, sep, &brkt);
	 line; line = strtok_r(NULL, sep, &brkt)){

		printf("\nKEY WORDS:> %s\n", line);

		//make sure a query is valid and return an array of the query
		validateQuery(line);
	}
}

//determine if a line in the query input is valid
void validateQuery(char *line){

	line= NormalizeWord(line);
	printf("Query: %s\n", line);

	int pos=0;
    char *Word;
    int wordCount=0;

	//make sure all chars are letters in each query
	int lineIndex;
	for(lineIndex=0; lineIndex<strlen(line); lineIndex++){

		if(line[lineIndex]!='\n' && !isalpha(line[lineIndex]) &&
		 line[lineIndex]!=' ' && line[lineIndex]!=0){
			fprintf(stderr, "ERROR: Bad character '%c' in query.\n", line[lineIndex]);
			return;
		}
	}

     //get number of words in line
     while((pos= GetNextWord(line, pos, &Word)) != -1){
      	free(Word);
      	wordCount++;
    }
    //init array of words for the line	
	char **wordArray= (char **) malloc(sizeof(char *) *wordCount);

	pos=0;
	int charIndex=0;

     //get number of words in line
     while((pos= GetNextWord(line, pos, &Word)) != -1){
      	//copy word in malloced array
     	wordArray[charIndex]=Word;
     	charIndex++;
    }

	//make sure and operaters aren'tn't first or last
	if(!strcmp(wordArray[0], "and")){
		fprintf(stderr, "ERROR: 'and' can not be first word in query\n");
		return;
	}
	else if(!strcmp(wordArray[0], "or")){
		fprintf(stderr, "ERROR: 'or' can not be first word in query\n");
		return;
	}
	else if(!strcmp(wordArray[wordCount-1], "and")){
		fprintf(stderr, "ERROR: 'and' can not be last word in query\n");
		return;
	}
	else if(!strcmp(wordArray[wordCount-1], "or")){
		fprintf(stderr, "ERROR: 'or' can not be last word in query\n");
		return;
	}

	//loop through all words
	for(int a=1; a<wordCount-1; a++){

		char *Word1=wordArray[a];
		char *Word2=wordArray[a+1];

		//check for adjacent operators
		if((!strcmp(Word1, "and") || !strcmp(Word1, "or")) && (!strcmp(Word2, "and") || !strcmp(Word2, "or"))){

			fprintf(stderr, "ERROR: can't have operators '%s' and '%s' adjacent to each other\n", Word1, Word2);
			return;
		}
	}

	runQuery(wordArray, wordCount);
	if(wordCount!=1){
		free(wordArray);
	}
}

//score all the docs in the crawler doc directory 
void runQuery(char **words, int wordCount){

	int wordIndex=0;
	int orCount=0;

	//get orCount
	for(wordIndex=0; wordIndex<wordCount; wordIndex++){

		//check if next word is or
		if(!strcmp(words[wordIndex], "or")){
			orCount++;
		}
	}

	//make array of int for count of words in each array
	int counts[orCount+1];

	//create arrays of arrays of arrays of chars
	char ***andArrays= malloc((orCount+1)*sizeof(char **));

	//if there were no or's add array to bag
	if(orCount==0){
		andArrays[0]=words;
		counts[0]=wordCount;

	}

	//there were ors
	//split words array where there is an or
	else{

		wordIndex=0;
		int prevIndex=0;
		char **andArray;
		int orIndex=0;

		//malloc char arrays in between every or
		for(wordIndex=0; wordIndex<wordCount; wordIndex++){

			//check if next word is or
			if(!strcmp(words[wordIndex], "or")){

				//malloc memory for words in between ors
				andArray = malloc((wordIndex-prevIndex) * sizeof(char*));
				memcpy(andArray, words+prevIndex,  (wordIndex-prevIndex)* sizeof(char*));

				andArrays[orIndex]= andArray;

				counts[orIndex]=(wordIndex-prevIndex);
				prevIndex=wordIndex+1;
				orIndex++;
				free(words[wordIndex]);
			}
		}

		//malloc memory for words in between ors
		andArray = malloc((wordCount-prevIndex) * sizeof(char*));
		memcpy(andArray, words+prevIndex,  (wordCount-prevIndex)* sizeof(char*));

		andArrays[orIndex]= andArray;
		counts[orIndex]=(wordIndex-prevIndex);

	}

	scoreQuery(andArrays, orCount+1, counts);
}

//score all the arrays in the bag
void scoreQuery(char ***andArrays, int arraySize, int *wordCounts){

	//if there were no or's go straing to get results
	if(arraySize==1){
		counters_t *andMatch= getAndScore(andArrays[0], wordCounts[0]);
		if(andMatch!=NULL){
			printResults(andMatch);
		}
		else{
			printf("No Results for search\n");
		}
	}
	//more than one or
	else{
		counters_t *findMatch=NULL;
		counters_t *tempMatch=NULL;

		//loop through a wordArrays
		for(int arrayIndex=0; arrayIndex<arraySize-1; arrayIndex++){

			//check if first iteration
			if(findMatch==NULL){
				tempMatch= getAndScore(andArrays[arrayIndex], wordCounts[arrayIndex]);
			}

			//make sure andArray score was found
			if(tempMatch!=NULL){

				//get find match
				findMatch= getAndScore(andArrays[arrayIndex+1], wordCounts[arrayIndex+1]);

				if(findMatch!=NULL){
					match_union(tempMatch, findMatch);
					tempMatch=findMatch;
				}
			}
		}

		//print results
		if(tempMatch!=NULL){
			printResults(findMatch);
		}
		else if(findMatch!=NULL){
			printResults(tempMatch);
		}
		else{
			printf("No Results for search\n");
		}
	}

	//free all memory
	for(int r=0; r<arraySize; r++){
		for(int j=0; j<wordCounts[r]; j++){
			free(andArrays[r][j]);
		}
		free(andArrays[r]);
	}
	free(andArrays);
}

//get the score of a single word array
counters_t *getAndScore(char **words, int wordCount){
	counters_t *findMatch;
	counters_t *tempMatch;

	//if one element in array return counters for it
	if(wordCount==1){
		return index_find(Index, words[0]);
	}

	int wordIndex;

	//loop through words until an and is found
	for(wordIndex=0; wordIndex<wordCount-1; wordIndex++){

		if(strcmp(words[wordIndex], "and")){

			tempMatch= index_find(Index, words[wordIndex]);;

			//word was found and is not a 'and'
			if(tempMatch!=NULL){
				
				//check if next word is first counter
				if(!strcmp(words[wordIndex+1], "and")){
					findMatch= index_find(Index, words[wordIndex+2]);

					//make sure find match counters exists
					if(findMatch!=NULL){
						match_intersect(findMatch, tempMatch);
						match_intersect(tempMatch, findMatch);
					}
				}

				//next word was another word and current isn't and
				else{

					findMatch= index_find(Index, words[wordIndex+1]);

					//make sure current word exists in findMatch counters
					if(findMatch!=NULL){
						match_intersect(findMatch, tempMatch);
						match_intersect(tempMatch, findMatch);
					}
				}
			}
		}
	}

	if(findMatch!=NULL){
		return findMatch;
	}

	return NULL;
}

//print the results of the query
void printResults(counters_t *resultCounters){
	getResultCount(resultCounters);

	aResult_t *results[numResults];
	initResults(resultCounters, results);

	aResult_t sortResults[numResults];

	//set sort rsults arrau
	for(int i=0; i<numResults; i++){ 
	    sortResults[i].score = results[i]->score;
	    sortResults[i].docID = results[i]->docID; 
	    free(results[i]);        
	}

	qsort(sortResults, numResults, sizeof(aResult_t), cmpfunction);

	printf("Matches %i documents (ranked):\n", numResults);

	//set sort rsults arrau
	for(int i=0; i<numResults; i++){ 
	    printURLscore(sortResults[i].docID, sortResults[i].score);       
	}

	numResults=0;	//reinit global var
}

//print score and url of found page
void printURLscore(int docID, int score){

	//get the url of the document
	char filename[100];
	//get proper filename
    sprintf(filename, "%s/%d", pageDirectory, docID);

    //create and write to file
    FILE *fp= fopen(filename, "r");

    //make sure file pointer works properly and exists
    //returns 1 if doesn't exists or can't be opened
    if(fp==NULL){
      fprintf(stderr, "ERROR: couldn't open document %s\n", filename);
      exit(2);
    }

    else{

    	//get url from doc and close it
    	char *url=readuntil(fp, stopfunc);
    	fclose(fp);

    	printf("score  %i doc   %i: http:%s\n", score, docID, url);
    	free(url);
    }
}

//compare two scores of a result
static int cmpfunction(const void * a, const void * b){
	int score1= ((aResult_t *)a)->score;
	int score2= ((aResult_t *)b)->score;

	return score2 - score1;
}


//get number of results
aResult_t **initResults(counters_t *resultS, aResult_t **results){

	counters_iterate(resultS, initARusult, results);

	initResultCount=0;
	return results;
}

//helper function to get number of results
static void initARusult(void *arg, int key, int count){

	aResult_t **results= (aResult_t **) arg;

	results[initResultCount]=aResult_new(key, count);
	initResultCount++;
	
}

//get number of results
int getResultCount(counters_t *results){

	counters_iterate(results, resultCount_helper, &numResults);
	return numResults;
}

//helper function to get number of results
static void resultCount_helper(void *arg, int key, int count){
    numResults++;
}

//create a new result item
aResult_t *aResult_new(int ID, int Score){

	aResult_t *aResult= malloc(sizeof(aResult_t));
	if (aResult == NULL) {
		return NULL; // error allocating aResult
	}

	aResult->docID=ID;
	aResult->score=Score;

	return aResult;
}

//figure out if parameters are valid
void parseParams(int argc, char *argv[]){

  //make sure right number of paramters
  if(argc!=3){
    fprintf(stderr, "ERROR: Please use 2 paramters to the querier command.\n");
    exit(1);
  }

    //check if argv[1] is a pre-existing directory
     DIR* dir = opendir(argv[1]);
     if(!dir){
     	//dir doesn't exists
     	fprintf(stderr, "ERROR: Directory %s does not exist.\n", argv[1]);
      	exit(1);
     }
     free(dir);

    char testfile2[100];
    sprintf(testfile2, "%s", argv[2]);

    FILE *fp;
    fp = fopen(testfile2, "r");
    if(fp == NULL){
      fprintf(stderr, "ERROR: IndexFile %s does not exist.\n", argv[2]);
      exit(1);
    }

    fclose(fp);
    pageDirectory= argv[1];
    indexFileName= argv[2];
}

//get the stdin for the query
char * getStdin(){

    char next;
    int z=0;

    //read from stdIn to buffer
    while((next=getchar())!=EOF){

        stdInBuff[z]=next;
        z++;
    }
    return stdInBuff;
}

//match two counter objects
void match_intersect(counters_t *A, counters_t *B){
     counters_iterate(A, intersect_helper, B);
}

//loop through one counter and figure out which docunents have the same
static void intersect_helper(void *arg, int key, int count){
     counters_t *B= arg;
     counters_set(B, key, min(count, counters_get(B, key)));
}

//union the counts of an or
static void match_union(counters_t *A, counters_t *B){
     counters_iterate(A, union_helper, B);
}

static void union_helper(void *arg, int key, int count){
    counters_t *B=arg;
    counters_set(B, key, count+counters_get(B, key));
}

//see which int is smaller for AND queries
static int min(int a, int b){
	if(a>b){
		return b;
	}
	return a;
}
