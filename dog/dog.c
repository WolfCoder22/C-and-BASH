#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>
#include <unistd.h>
#include "dog.h"

//declare global variabls
bool nTyped=false;
bool mTyped= false;
bool MTyped= false;
bool needLineNum= true;

char *mapFileName = NULL;
int firstFileIndex;
int lineCount=1;

//empty char arrays buffers for encrption/decryption and getting stdin
char originalChars[BUFFER_SIZE];
char newChars[BUFFER_SIZE];
char stdInBuff[BUFFER_SIZE];
char argvTemp[BUFFER_SIZE][BUFFER_SIZE];

char holderChar; 
char *standardIn;

//main function for dog
int main(int argc, char *argv[]){

    //Check if help option entered
    if((argc==2) && (strcmp("--help",argv[1]) == 0)){

        //open mapFile
        int e=0;
        FILE *fp = fopen("README","r");
        while ((e = fgetc(fp)) != EOF){ 

            //cast int to char and print
            char c = e;
            printf("%c", c);

         }   

        return 0;
    }

    //check for option -n -M and -m(last two requre file after)
    char *commands= ":nm:M:";

    getOptions(argc, argv, commands);

    checkOptions(argc, argv);

    //check if encrypting or decrypting and get the arrays for it
    if(mTyped || MTyped){

        //create arrays for chacater files
        getCharChangeVals(mapFileName);
    }

    //check if no files were entered as input
    if(argc== firstFileIndex){

        //get the strdin and display the file
        standardIn= getStdin();
        display_file(standardIn);

    }

    //files were entered
    else{
        int v;

        //loop through files entered on command line
        for(v= firstFileIndex; v<argc; v++ ){

            //check if it's - indicating a special chacter
            if(strcmp("-",argv[v]) == 0){
                
                //get the strdin and display the file
                standardIn= getStdin();
                display_file(standardIn);
            }

            //option doesn't represent stdin
            else{

                display_file(argv[v]);
            }

        }
    }
}



//turn booleans to true if options entered properly
//exit and print errors if not
void getOptions(int argc, char *argv[], char *commands){

    //make temp argv since is changed after getopt
    int i;
    for(i=0; i<argc; i++){
        //argvTemp[i]=argv[i];
        strcpy(argvTemp[i], argv[i]);
    }

    int c;
    while ((c = getopt (argc, argv, commands)) != -1)
        switch (c){

             case 'm':

                 //check is M was already entered
                 if(!MTyped){

                    mTyped = true;  //note that it was entered
                    mapFileName=optarg; //get file for option parsing
                    }

                  //print error saying can't do both
                  else{
                    printf("ERROR: -M option already entered. Can't use -m too");
                    exit(1);
                  }

                break;

            case 'M':
                 //check is m was already entered
                 if(!mTyped){

                    MTyped = true;  //note that it was entered
                    mapFileName=optarg; //get file for option parsing
                }

                //print error saying can't do both
                else{
                    printf("ERROR: -m option already entered. Can't use -M too");
                    exit(1);
                  }
                        
                 break;

             case 'n':
                 nTyped = true;
                 break;

            case ':':
                printf("ERROR: The option -%c lacks an arguement", optopt);
                exit(1);
                break;

         //handle eerors messages
             default:
                fprintf(stderr, "Usage: dog [-n] [-m/M argument]\n");
                exit(1);

        }

    firstFileIndex=optind;
}    

void getCharChangeVals(char *mapFileName){
    printf("%s", mapFileName);
    //open mapFile
    FILE *fp = fopen(mapFileName,"r");

    char ch;
    int lines=0;

    //get number of lines in file
    while(!feof(fp)){
        ch = fgetc(fp);
        if(ch == '\n'){
            lines++;
            }
    }

    //make sure file is at least 26 lines long
    //27 instead of 26 since including the \n at the last line
    if(lines<27){
        printf("ERROR: Mapfile must contain at least 26 lines\n");
        exit(2);
    }

    //read mapfileName
    if((fp=fopen(mapFileName, "r")) == NULL) {
       printf("ERROR: couldn't open file %s of char encodings", mapFileName);
       exit(3);
    } 
    //file was read
    else {

        //get each line in the file
        int i=0;
        char line[BUFFER_SIZE];
        while (fgets(line, sizeof(line), fp)) {

            int size= strlen(line);

            //check if it's a feasble line for encoding of ASCII char-space-ASCII char-/n
            if(size==4 && line[1]==' ' && line[0]!=' ' && line[0]!=' ' && line[3]=='\n' ){

                //if both letters 
                if(isalpha(line[0]) && isalpha(line[2])){

                    //make sure both chars are lower or both upper
                    if(isupper(line[0])!=isupper(line[2])){
                        printf("ERROR: Improperly formatted mapfile\n");
                        printf("If encoding a letter char to a letter char make sure to keep letter case");
                        printf("Check line number %i in %s", i+1, mapFileName);
                        exit(2);
                     }

                 }

                 //check if character already entered in mapfile
                 int k;
                 int charsEncoded=strlen(originalChars);

                 for(k=0; k<charsEncoded; k++){
                    //check if double first char anywhere in file
                    if(originalChars[k]==line[0]){
                        printf("ERROR: double original char in mapfile\n");
                        printf("Check first charcter in line number %i and %i in %s", i+1, k+1, mapFileName);
                        exit(2);

                    }

                    //check if double second char anywhere in file
                    if(newChars[k]==line[2]){
                        printf("ERROR: double decoding char in mapfile\n");
                        printf("Check second charcter in line number %i and %i in %s", i+1, k+1, mapFileName);
                        exit(2);
                    }
                 }

                 //update char array values for encoding and decoding
                 originalChars[i]=line[0];
                 newChars[i]=line[2];
                 i++; //update index
            }

            //error wrong format of line
            //spaces in first or third char

            else{
                printf("ERROR: Improperly formatted mapfile\n");
                printf("-make sure each line is || 'c'' ''c''\\n'\n");
                printf("Check line number %i in %s", i+1, mapFileName);
                exit(2);
            }
        }
    //close the file
    fclose(fp);
    }
}

//method for displaying the contents of a file
void display_file(char *file_name){

    //open mapFile
    FILE *fp = fopen(file_name,"r");

    //read file
    if(fp == NULL) {
       printf("ERROR: File %s does not exist in directory.", file_name);
       exit(3);
    } 
    //opened properly
    else{

        //notify user if printing file encoded or decoded
        if(mTyped){
            printf("Showing contents from file named -%s\nEach charater encoded from file -%s \n", file_name, mapFileName);
        }
        else if(MTyped){
            printf("Showing contents from file named -%s\nEach charater decoded from file -%s \n", file_name, mapFileName);
        }
        else{
            printf("Showing contents of file named -%s\n", file_name);
        }
        printf("------------------------------------\n");

        //print first linecount if ntyped
        if(needLineNum && nTyped){
                printf("%i   ", lineCount); 
                needLineNum= false;
            }


         int k; //vars for encoding/decoding looping
         int charsEncoded=strlen(originalChars);

        //go through every charcter
        int i;
        while ((i = fgetc(fp)) != EOF){    // read character from file until EOF
            
            //cast int to char
            char c = i;

            //if -m option was entered
            if(mTyped){

                //loop through all characters that have be singaled be be encoded
                 for(k=0; k<charsEncoded; k++){
                    //if the current has been signaled
                    if(originalChars[k]==c){

                        //encode character c
                        c=newChars[k];
                    }
                }
            }
            //if -m option was entered
            if(MTyped){

                //loop through all characters that have be singaled be be encoded
                 for(k=0; k<charsEncoded; k++){

                    //if the current has been signaled
                    if(newChars[k]==c){

                        //decode character c
                        c=originalChars[k];
                    }
                }
            }

            //check if n option turned on
            if(nTyped){ 

                //there's a new line
                if(c=='\n'){

                    //output with line number 
                    lineCount++;    //update line count
                     printf("\n%i   ", lineCount); 

                }

                else{
                    printf("%c", c); // output character
                }
            }   

            else{
                printf("%c", c); // output character
            }
        }
        needLineNum= true;  //update bool to print line on start of next file input
        printf("\n\n");
        fclose(fp);
    }
}

char * getStdin(){

    char next;
    int z=0;

    //read from stdIn to buffer
    while((next=getchar())!=EOF && next!='\n'){

        stdInBuff[z]=next;
        z++;
    }
    return stdInBuff;
}

void checkOptions(int argc, char *argv[]){


    int i;//loop through all commands
    for(i=1; i<argc; i++){
        
        //check if -n before files
        if(strcmp(argvTemp[i], "-n")==0){

            if((!(i==1) || (!(mTyped||MTyped) && i==3))){
                printf("Error: The option -n must appear before files \n");
                exit(4);
            }

        }

        //check if -m before files
        //printf("%i is bool \n", strcmp(argv[i], "-m")==0);
        if(strcmp(argvTemp[i], "-m")==0 && !(i==1 || (nTyped && i==2))){
            
                printf("Error: The option -m must appear before files \n");
                exit(4);
            
        }

        //check if -M before files
        if(strcmp(argvTemp[i], "-M") ==0&& !(i==1 || (nTyped && i==2))){

            printf("Error: The option -M must appear before files \n");
            exit(4);
        }
    }

}

        

      
