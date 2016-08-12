# Makefile for 'set' module
#
# Alex Wolf, july 2016

# PROG =
OBJS = counters.o
LIBS =

MAKE = make
CC = @gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb

# I have no tester program
# $(PROG): $(OBJS)
# 	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

counters.o: counters.h

.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o