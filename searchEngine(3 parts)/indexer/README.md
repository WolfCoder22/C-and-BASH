# CS50 Labs 4-5-6: Tiny Search Engine


README for indexer.c example
Alex Wolf, August 2016

Compiling:
	make

Usage:
/* 
 * indexer.c - substitute text into a template file
 * 
 * usage: look at docs from craweler and index them by words, docID, and word count into a new file
 * output:
 *	 -Index file created from crawler docs
 * stdin: None
 * stdout: none
 * stderr: error messages
 *
 * Alex Wolf, July 2016
 */

Example command lines:

  ./indexer ../data indexFileName
  	-params
  	1. directory of location of crawler files
  	2. name for the index file
  

Exit status:
  0 - success
  1 - incorrect paramters
  2 - error mallocing data

Assumptions:

- The user knows that if he/she uses an existing file as the second paramter name it will be over written

"Non"-Assumptions:
-none
