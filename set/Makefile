# Makefile for 'set' module
#
# Alex Wolf, July 2016

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -lm 
PROG = settest
OBJS = settest.c set.c

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

settest.o: set.h
set.o: set.h

clean:
	rm -f *~
	rm -f *.o
	rm -rf $(PROG).dSYM
	rm -f $(PROG)