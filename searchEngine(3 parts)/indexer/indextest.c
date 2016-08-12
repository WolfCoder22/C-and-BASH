/* 
 * indextest - load an index, and save it, to test those functions
 *
 * 
 * Alex Wolf, Summer 2016
 */

 //program includes
 #include <stdlib.h> 
 #include <stdio.h>                        
 #include <stdbool.h>
 #include <string.h>
 #include "../common/index.h"
 #include "../common/file.h"

/******local functions*****/
//figure out if parameters are valid
void parseParams(int argc, char *argv[]);
bool fileExists(char *oldFileName);

/***local vars***/
char *oldFileName;
char *newFileName;
index_t *index1;


//main function for indexer
int main(int argc, char *argv[]){
  
	//figure out if parameters are valid
	parseParams(argc, argv);

	//init index size has big as of for indexer
	index1= index_new(1000);

	//init index and load data to it from oldFileIndex
	index1=loadIndex(oldFileName, index1);

  FILE *fp= fopen(newFileName, "w+");
	index_save(fp, index1);

	index_delete(index1);

  fclose(fp);
	return 0;
}


//figure out if parameters are valid
void parseParams(int argc, char *argv[]){

	//make sure right number of paramters
  	if(argc!=3){
    	fprintf(stderr, "ERROR: Please use 2 paramters to the indexertest command.\n");
    	exit(1);
  	}

  	//if oldfile don'st exists print error
  	if(!fileExists(argv[1])){
  		fprintf(stderr, "ERROR: Make sure param 2 is the filename of an existing indexer file.\n");
    	exit(1);

  	}
  	//file exists; set local vars from params
  	else{
  		oldFileName=argv[1];
  		newFileName=argv[2];
  	
  	}
}

//test if a file exists 
//return true if exists, false otherwise
bool fileExists(char *oldFileName){

    FILE *fp;
    fp = fopen(oldFileName, "r");

    if(fp == NULL){
      fclose(fp);
      return false;
    }
    else{
      fclose(fp);
      return true;
    }
}
