/* 
 * indexer - a simple web indexer
 *
 * Alex wolf, Summer 2016
 */

//include header file data
#include "indexer.h"

//local funcs
//test if a file exists for dir counting purposes
bool fileExists(int docID);

//global vars
char *pageDirectory;
char *newFileName;
index_t *index;

//main function for indexer
int main(int argc, char *argv[]){

	// printf("\n");
 //   printf("gets here");
  parseParams(argc, argv);  //parse commands 
  index=index_new(1000);  //init data structs for crawl
  
  //read all files in database
  //assumes in proper format from crawler command
  int docID;
  for(docID=1; fileExists(docID); docID++){
    
    //read Document and update inndex data
    index_document(docID, pageDirectory, index);
  }

	   //create and write to file
    FILE *fp= fopen(newFileName, "w+");

    //write index file data to new file with argv[2] name
    index_save(fp, index);

    fclose(fp);

  	//free hashtable
  	index_delete(index);
}



//figure out if parameters are valid
void parseParams(int argc, char *argv[]){

  //make sure right number of paramters
  if(argc!=3){
    fprintf(stderr, "ERROR: Please use 2 paramters to the indexer command.\n");
    exit(1);
  }

    //check if argv[1] is a pre-existing directory
     FILE *fp;

    char testfile[100];
    sprintf(testfile, "%s/1", argv[1]);

    fp = fopen(testfile, "w");
    if(fp == NULL){
      fprintf(stderr, "ERROR: Directory %s does not exist.\n", argv[1]);
      exit(1);
    }

    fclose(fp);

    pageDirectory= argv[1];
    newFileName= argv[2];
}

//test if a file exists for dir counting purposes
bool fileExists(int docID){

    char testfile[100];
    sprintf(testfile, "%s/%i", pageDirectory, docID);

    FILE *fp;
    fp = fopen(testfile, "r");

    if(fp == NULL){
      return false;
    }
    else{
      fclose(fp);
      return true;
    }
}



