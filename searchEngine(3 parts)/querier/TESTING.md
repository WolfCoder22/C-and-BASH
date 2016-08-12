##### TESTING for querier.c 
### Alex Wolf, August 2016

#### Test noticing when the wrong parameters were enetered:####

## incorrect params

1.IndexFile doesn't exits
wolfiesmac:querier alexwolf$ cat stdin | ./querier ../data ../indexer/indexFileNam

ERROR: IndexFile ../indexer/indexFileNam does not exist.
wolfiesmac:querier alexwolf$ 

2.Crawler data file doesn't exists
wolfiesmac:querier alexwolf$ cat stdin | ./querier ../dat ../indexer/indexFileName

ERROR: Directory ../dat does not exist.
wolfiesmac:querier alexwolf$ 

3. Incorrect number of Params
wolfiesmac:querier alexwolf$ cat stdin | ./querier ../data

ERROR: Please use 2 paramters to the querier command.
wolfiesmac:querier alexwolf$ 

##test whether the query from stdin is valid

1. bad chacrters in query
wolfiesmac:querier alexwolf$ cat stdin | ./querier ../data ../indexer/indexFileName

KEY WORDS:> Word anD seArc8h
Bad character '8' in query.
wolfiesmac:querier alexwolf$ 

2. operator first word in query
wolfiesmac:querier alexwolf$ cat stdin | ./querier ../data ../indexer/indexFileName

KEY WORDS:> aND seArch
ERROR: 'and' can not be last word in query
wolfiesmac:querier alexwolf$ 

3. operator last word in query
wolfiesmac:querier alexwolf$ cat stdin | ./querier ../data ../indexer/indexFileName

KEY WORDS:> seArch dartmouth oR
ERROR: 'or' can not be last word in query
wolfiesmac:querier alexwolf$ 

4. adjacent operators
KEY WORDS:> seArch and dartmouth or and dog
Query: search and dartmouth or and dog
ERROR: can't have operators 'or' and 'and' adjacent to each other
wolfiesmac:querier alexwolf$ cat stdin | ./querier ../data ../indexer/indexFileName

KEY WORDS:> seArch and dartmouth and and dog
Query: search and dartmouth and and dog
ERROR: can't have operators 'and' and 'and' adjacent to each other
wolfiesmac:querier alexwolf$ 

####Test memory leaks and correct output####
[flume:querier] 292) cat stdin | valgrind --leak-check=full --show-reachable=yes ./querier  ../data ../data/index
==26356== Memcheck, a memory error detector
==26356== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==26356== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==26356== Command: ./querier ../data ../data/index
==26356== 


KEY WORDS:> dartmouth or computer science or programming or unix or doug mcilroy 
Query: dartmouth or computer science or programming or unix or doug mcilroy 
Matches 10 documents (ranked):
score  39 doc   6: http:http://old-www.cs.dartmouth.edu/~cs50/Resources/
score  14 doc   7: http:http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score  9 doc   8: http:http://old-www.cs.dartmouth.edu/~cs50/Reading/
score  9 doc   2: http:http://old-www.cs.dartmouth.edu/~xia/
score  4 doc   1: http:http://old-www.cs.dartmouth.edu/~cs50/index.html
score  4 doc   10: http:http://old-www.cs.dartmouth.edu/~cs50/
score  2 doc   3: http:http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score  1 doc   4: http:http://old-www.cs.dartmouth.edu/~cs50/Project/
score  1 doc   5: http:http://old-www.cs.dartmouth.edu/~cs50/Labs/
score  1 doc   9: http:http://old-www.cs.dartmouth.edu/~cs50/Lectures/
==26356== 
==26356== HEAP SUMMARY:
==26356==     in use at exit: 0 bytes in 0 blocks
==26356==   total heap usage: 18,534 allocs, 18,534 frees, 386,310 bytes allocated
==26356== 
==26356== All heap blocks were freed -- no leaks are possible
==26356== 
==26356== For counts of detected and suppressed errors, rerun with: -v
==26356== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

#### Testing mutiple queries
[flume:querier] 410) cat stdin | valgrind --leak-check=full --show-reachable=yes ./querier  ../data ../data/index
==2299== Memcheck, a memory error detector
==2299== Copyright (C) 2002-2015, and GNU GPL'd, by Julian Seward et al.
==2299== Using Valgrind-3.11.0 and LibVEX; rerun with -h for copyright info
==2299== Command: ./querier ../data ../data/index
==2299== 


KEY WORDS:> dartmouth or computer science or programming or unix or doug mcilroy 
Query: dartmouth or computer science or programming or unix or doug mcilroy 
Matches 10 documents (ranked):
score  39 doc   6: http:http://old-www.cs.dartmouth.edu/~cs50/Resources/
score  14 doc   7: http:http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score  9 doc   8: http:http://old-www.cs.dartmouth.edu/~cs50/Reading/
score  9 doc   2: http:http://old-www.cs.dartmouth.edu/~xia/
score  4 doc   1: http:http://old-www.cs.dartmouth.edu/~cs50/index.html
score  4 doc   10: http:http://old-www.cs.dartmouth.edu/~cs50/
score  2 doc   3: http:http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score  1 doc   4: http:http://old-www.cs.dartmouth.edu/~cs50/Project/
score  1 doc   5: http:http://old-www.cs.dartmouth.edu/~cs50/Labs/
score  1 doc   9: http:http://old-www.cs.dartmouth.edu/~cs50/Lectures/

KEY WORDS:> dartmouth or yes and sudikoff
Query: dartmouth or yes and sudikoff
Matches 10 documents (ranked):
score  11 doc   7: http:http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score  7 doc   2: http:http://old-www.cs.dartmouth.edu/~xia/
score  3 doc   6: http:http://old-www.cs.dartmouth.edu/~cs50/Resources/
score  1 doc   1: http:http://old-www.cs.dartmouth.edu/~cs50/index.html
score  1 doc   10: http:http://old-www.cs.dartmouth.edu/~cs50/
score  1 doc   3: http:http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score  1 doc   4: http:http://old-www.cs.dartmouth.edu/~cs50/Project/
score  1 doc   5: http:http://old-www.cs.dartmouth.edu/~cs50/Labs/
score  1 doc   8: http:http://old-www.cs.dartmouth.edu/~cs50/Reading/
score  1 doc   9: http:http://old-www.cs.dartmouth.edu/~cs50/Lectures/

KEY WORDS:> unix
Query: unix
Matches 10 documents (ranked):
score  39 doc   6: http:http://old-www.cs.dartmouth.edu/~cs50/Resources/
score  14 doc   7: http:http://old-www.cs.dartmouth.edu/~cs50/Logistics/
score  9 doc   2: http:http://old-www.cs.dartmouth.edu/~xia/
score  8 doc   8: http:http://old-www.cs.dartmouth.edu/~cs50/Reading/
score  4 doc   1: http:http://old-www.cs.dartmouth.edu/~cs50/index.html
score  4 doc   10: http:http://old-www.cs.dartmouth.edu/~cs50/
score  2 doc   3: http:http://old-www.cs.dartmouth.edu/~cs50/Labs/Lab0-Preliminaries.html
score  1 doc   4: http:http://old-www.cs.dartmouth.edu/~cs50/Project/
score  1 doc   5: http:http://old-www.cs.dartmouth.edu/~cs50/Labs/
score  1 doc   9: http:http://old-www.cs.dartmouth.edu/~cs50/Lectures/
==2299== 
==2299== HEAP SUMMARY:
==2299==     in use at exit: 0 bytes in 0 blocks
==2299==   total heap usage: 17,225 allocs, 17,225 frees, 387,938 bytes allocated
==2299== 
==2299== All heap blocks were freed -- no leaks are possible
==2299== 
==2299== For counts of detected and suppressed errors, rerun with: -v
==2299== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

[flume:querier] 411) 
