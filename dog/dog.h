#include <stdbool.h>
#include <string.h>

#define BUFFER_SIZE 255

//function declarations for chill.c
void getCharChangeVals(char *mapFile);
void getOptions(int argc, char *argv[], char *commands);
void checkOptions(int argc, char *argv[]);
void display_file(char *file_name);
char * getStdin();
