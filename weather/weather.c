
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>


#include <curl/curl.h>
#include "weather.h"
#include "mycurl.h"

/* global variables defined */

int main(int argc, char *argv[]){

    printf("\n");
    
    /* local variables */
    char *code;
    char APIurl[BUFFER_SIZE];
    char webBuff[BUFFER_SIZE];
    char *xml="<?xml";

    /* check args */
    if(argc!=2){
        printf("ERROR: Weather only takes one argument. Currently have %i.\n",argc-1);
        printf( "Usage: weather [Four Letter uppercase letter station code]\n");
        exit(2);
    }
    
    else if(strlen(argv[1])!=4){
        printf("ERROR: First option must be four letter station code. Currently have %s.\n",argv[1]);
        printf( "Usage: weather [Four Letter uppercase letter station code]\n");
        exit(2);
    }
    //make sure code is all caps
    else{
        if(codeIsValid(argv[1])){
            printf("ERROR: Make sure station code is all uppercase letters. Currently have %s.\n",argv[1]);
            printf("Usage: weather [Four Letter uppercase letter station code]\n");
            exit(2);
        }
        else{
            code= argv[1];

            }
    }

     /* curl stuff */
     CURL *curl;
     CURLcode res;
     struct curlResponse s;

    /* create the URL from the format string */
    /* ... */

    /* Format string for the URL we will request */
    strcpy(APIurl, "http://w1.weather.gov/xml/current_obs/");
    strcat(APIurl, code);
    strcat(APIurl, ".xml");

    // allocate and initialize the output area 
    init_curlResponse(&s);

    /* initialize curl */
    curl = curl_easy_init();

    if(curl) {
        /* set the URL we want to visit */
        curl_easy_setopt(curl, CURLOPT_URL, APIurl);
        
        /* weather.gov expects USERAGENT option */
        curl_easy_setopt(curl, CURLOPT_USERAGENT, "cs50");
                
        /* set the function curl should call with the result */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);

        /* set the buffer into which curl should place the data */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);

        /* Perform the request, res will get the return code */
        res = curl_easy_perform(curl);

        /* always cleanup */ 
        curl_easy_cleanup(curl);

        /* check: if the weather station code wasn't found, */
        /* we get back HTML instead of XML  */
        if(res!=0){
            fprintf(stderr, curl_easy_strerror(res));
            return(2);
        }

        // /* make sure it worked */
        strncpy(webBuff, s.ptr, 5);
        for (int i = 0; i < 5; i++) {

            //if the charcteras are equal don't spit error
            if (s.ptr[i] != xml[i]) {
                printf("Error: The weather station code %s wasn't found\n", code);
                return 3;
            }
        }

         printf("Showing data for weather Station %s\n---------------------------------------\n", code);
    
        //desried data wanting to retrive from station
        char *webdata[] = {"credit","location", "station_id", "observation_time", 
        "weather", "temperature_string", "relative_humidity", "visibility_mi","wind_string"};

        //otherwise, select the desired output from the results 
        char *tokens =  strtok(s.ptr, "<>");

        while(tokens!=NULL){  //loop through all 
            tokens =  strtok(NULL, "<>\n"); //only get strings in <>


            if (tokens == NULL) {   //if we find what we want stop searching
                break;
            }

            for (int i = 0; i < 9; i++) {

                //only get desired data for wheathe station
                if (strcmp(webdata[i], tokens) == 0) {
                    tokens =  strtok(NULL, "<>\n"); 

                    printf("%s: %s\n", webdata[i], tokens);
                     break;
                }
             }  
        }
    }
    return 0 ;
}


/*  makes sure all chars are upper and letters
    return 1 on false
    return 0 on true;  */
int codeIsValid(char code[]){
    int a;
    int d=0;

    //loop through all chars in code
    for (a=0; a<4; a++){
        if(!(isalpha(code[a]) && isupper(code[a]))){
            d+=1;
        } 
        
    }
    return d;

}
