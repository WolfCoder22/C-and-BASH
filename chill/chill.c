#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include "chill.h"

//main function for chill
int main( int argc, char *argv[]){

	//check number of paramters
	int numParam= argc-1;

	//make sure no greater than 2 parameters enetered
	if(numParam > 2){
		printf("\nPlease enter no more than two paramters.\n");
	}

	//only temp entered
	else if (numParam == 1){

		float temp;
		float wind;
		//make sure temp is an digit
		if (!isdigit(*argv[1])) {
	 		printf("\nMake sure temperature entered is a digit.\n");
	 		return 0;
       	}

       	else{

	       	//cast parameter
	       	sscanf(argv[1], "%f", &temp);
       	}

       	//make sure temp is less than 50
       	if (temp >= 50){
       		printf("\nTemperature must be less than 50 degrees Fahrenheit.\n");
       		return 0;
       	}

       	//print collum names
		printf("\nTemp  Wind    Chill\n");
		printf("----    ----    -----\n");

		//print output for wind 5, 10, and 15
		for(wind=5; wind<20; wind+=5){
			
			//get and print chill
			float chill= getChill(temp, wind);
			printf("%g\t%g\t%g\n" , temp, wind, chill);

		}

	}

	//wind and temp given
	else if (numParam == 2){

		float temp;
		float wind;

		//make sure temp is an digit
		if (!isdigit(*argv[1])) {
	 		printf("\nMake sure Temperature entered is a digit.\n");
	 		return 0;
       	}
       	else{
       		//cast parameter
       		sscanf(argv[1], "%f", &temp);
       	}

       	//make sure temp is less than 50
       	if (temp >= 50){
       		printf("\nTemperature must be less than 50 degrees Fahrenheit.\n");
       		return 0;
       	}

       	//make sure temp is an digit
		if (!isdigit(*argv[2])) {
	 		printf("\nMake sure Wind velocity entered is a digit; Ex. 0.7 not .7 or a char\n");
	 		return 0;
       	}

       	else{
       		//cast parameter
       		sscanf(argv[2], "%f", &wind);
       	}

       	//make sure wind is greater than
       	if (wind<0.5){
       		printf("\nWind velocity must be greater than or equal to 0.5 MPH\n");
       		return 0;
       	}

        //print collum names
		printf("\nTemp  Wind    Chill\n");
		printf("----    ----    -----\n");

		//get and print chill
		float chill= getChill(temp, wind);
		printf("%g\t%g\t%g\n" , temp, wind, chill);
	}

	//no parameters were entered by the user
	else{

		//print collum names
		printf("\nTemp  Wind    Chill\n");
		printf("----    ----    -----\n");

		float temp;

		//compute wind chill for wind 5, 10, and 15 with temp ranges -10 (f) to 40(f) by 10's
		for(temp=-10; temp<50; temp+=10){

			float wind;
			printf("\n"); //line break to seperate temps
			
			for(wind=5; wind<20; wind+=5){
				
				//get and print chill
				float chill= getChill(temp, wind);
				printf("%g\t%g\t%g\n" , temp, wind, chill);

			}
			

		}
	}
}

//compute chill for temp and wind
float getChill(float temp, float wind){

	float chill = 35.74 + 0.6215*temp - 35.75*pow(wind,0.16) + 0.4275*temp*pow(wind,0.16);
	float t= ((int)(chill*10)) / 10.0;
	return t;
}



