# Makefile for 'counters' module
#
# Alex Wolf, July 2016

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -lm 
PROG = counterstest
OBJS = counterstest.c counters.c

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

counterstest.o: counters.h
counters.o: counters.h

clean:
	rm -f *~
	rm -f *.o
	rm -rf $(PROG).dSYM
	rm -f $(PROG)