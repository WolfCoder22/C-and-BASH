# Makefile for 'hashtable' module
#
# Alex Wolf July 2016

# PROG =
OBJS = jhash.o hashtable.o 
LIBS =

MAKE = make
CC = @gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

# I have no tester program
# $(PROG): $(OBJS)
# 	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

hashtable.o: ../set/set.h jhash.h hashtable.h

jhash.o: jhash.h


.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o