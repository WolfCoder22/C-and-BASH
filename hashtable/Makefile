# Makefile for 'hashtable' program
#
# Alex Wolf, July 2016

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb
PROG = hashtabletest
OBJS = hashtabletest.c hashtable.c ../set/set.c

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

hashtabletest.o: hashtable.h ../set/set.h
set.o: ../set/set.h

clean:
	rm -f *~
	rm -f *.o
	rm -rf $(PROG).dSYM
	rm -f $(PROG)