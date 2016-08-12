/*
Design.md for querier of TSE

Alex Wolf, August 2016
*/

#### Design.md for indexer of TSE

## Querier Requirements Spec
The TSE querier is a standalone program that reads the index file produced by the TSE indexer, and answers search queries submitted via stdin.

The querier shall:

execute from a command line with usage syntax
./querier pageDirectory indexFilename
where pageDirectory is a directory produced by the crawler, and
where indexFilename is an index file produced by the indexer.
load the index from indexFilename into an internal data structure.

read search queries from stdin, one per line, until EOF.
clean and parse each query according to the syntax described below.
print the ‘clean’ query for user to see.
use the index to identify the set of documents that satisfy the query, as described below.
if no documents satisfy the query, print No documents match.
otherwise, rank the resulting set of documents according to its score, as described below, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL. (Obtain the URL by reading the first line of the relevant document file from the pageDirectory.)
The querier shall validate its command-line arguments:

pageDirectory is a directory produced by the crawler, and
indexFilename is the name of a readable file.
The querier may assume that the input directory and files follow the designated formats.

pageDirectory has files named 1, 2, 3, …, without gaps.
The content of document files in pageDirectory follow the format as defined in the crawler specs; thus your code (to read a document file) need not have extensive error checking.
The content of the file named by indexFilename follows our Index file format; thus your code (to recreate an index structure by reading a file) need not have extensive error checking.

## Design Spec
1. load index file into index
2. use strtok and GetNextWord to have a cha** of each line in the query
-use counters iterate to merge word cunters
3. Search for 'or' in the query split the char **  into a char *** where or's are present
-use counters iterate to merge word cunters
4. loop through all and arrays of char** and get the rank
5. loop through the or's and get the ranked of all combined
6. once have final rank sort it using qsort then print using counters interater 

## Testing plan
1.Make sure params are correct
2. Make sure correct get the char*** array of andWordArrays
3. Make sure the getAndScore function works properly
4. Make sure scoreQuery function works properly
5. Make sure finallized query of coununters is sorted by rank
6. print the query properly
7. Test for mutiple types of queries
8. Check for memory leaks

## To test the querier yourself do the command within the querier directory
 cat stdin | ./querier  ../data ../data/index
 -stdin is a file in the dir with the query



	
