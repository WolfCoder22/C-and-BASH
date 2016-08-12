#### README for querier.c example
#### Alex Wolf, August 2016

## Compiling:
	make

## Usage:
/* 
 * querier.c - query a url for given words and rank them in order
 * 
 * output:
 *   read the stdin, copying it to stdout, but wherever the 
 *   target text is found, replace it with the contents of the 
 *   replacement file.
 * stdin: the query; each line is a new query; ignore new lines and spaces
 * stdout: the query with ranked urls based off words
 * stderr: error messages
 *
 */

## Example command lines:

  1. cat stdin | ./querier  ../data ../data/index
  2. ./querier  ../data ../data/index

## Exit status:

  0 - success
  1 - incorrect number of arguments
  2 - bad files in craeler data directory

## Assumptions:

- the data from the crawler and the indexFile from indexer from the the same seed url and have he corect data

## "Non"-Assumptions:

- stdin and replacement file may contain any number of spaces and new lines

