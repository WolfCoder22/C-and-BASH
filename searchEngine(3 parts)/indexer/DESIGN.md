/*
Design.md for indexer of TSE

Alex Wolf, August 2016
*/

######Design.md for indexer of TSE

###Requirement Spec
The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.

The indexer shall:

execute from a command line with usage syntax
./indexer pageDirectory indexFilename
where pageDirectory is a directory produced by the crawler, and
where indexFilename is the name of a file into which the index should be written.
read documents from the pageDirectory, each of which has a unique document ID,
wherein the document id starts at 1 and increments for each new document,
and the filename is of form pageDirectory/id,
and the first line of the file is the URL,
and the second line of the file is the depth,
and the rest of the file is the page content.
build an inverted-index data structure mapping from words to (documentID, count) pairs, wherein each count represents the number of occurrences of the given word in the given document. Ignore words with fewer than three characters, and NormalizeWord before indexing.
create a file indexFilename and write the index to that file, in the format described below.
The indexer shall validate its command-line arguments:

pageDirectory is a directory produced by the crawler (see below), and
indexFilename is the name of a writeable file.
The indexer may assume that

pageDirectory has files named 1, 2, 3, …, without gaps.
The content of files in pageDirectory follow the format as defined in the crawler specs; thus your code (to create a WebPage by reading a file) need not have extensive error checking.
The index tester shall:

execute from a command line with usage syntax
./indextest oldIndexFilename newIndexFilename
where oldIndexFilename is the name of a file produced by the indexer, and
where newIndexFilename is the name of a file into which the index should be written.
load the index from the oldIndexFilename into an inverted-index data structure.
create a file newIndexFilename and write the index to that file, in the format described below.
It need not validate its command-line arguments other than to ensure that it receives precisely two arguments; it may simply try to open the oldIndexFilename for reading and, later, try to open the newIndexFilename for writing. Your indextest.sh shall then compare the two index files to test they are identical.

The index tester may assume that

The content of the index file follows the format specified below; thus your code (to recreate an index structure by reading a file) need not have extensive error checking.
Index file format

The indexer writes the inverted index to a file, and the querier reads the inverted index from a file; the file must be in the following format.

one line per word, one word per line
each line provides the word and one or more (docID, count) pairs, in the format
word docID count [docID count]…
where word is a string of lower-case letters,
where docID is a positive non-zero integer,
where count is a positive non-zero integer,
where the word and integers are separated by spaces.

###Design Spec
1. Create and index struct in the common directory
..1. Created from hashtable struct but has different methods
..2. key of index is a word from all the html docs fro crawler
2. The void * data in the index is a counters object
..1.the key is the docID
..2. the data is the count
..* updatye the count each time the word is found in the doc
3. Read all data from the index and write it to a file inputted as a parameter

###Testing plan
1. Make sure data inserted into index properly
2. Make sure data from index written to file properly
3. Create indextest doc for reading data from index file and put back into index
..* write this data to another file
4. write a shell script doing indexer and indextest and check to see if output files equal
..*has to sort files first
..*if data equal will print to stdout and will do the same if not
5. Use valgrind to check memory leaks

###To test the indexer yourself do the command within the indexer directory
1. ./indextestShell ../data indexFileName indexTestFileName



	
