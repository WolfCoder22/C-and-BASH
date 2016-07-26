# Makefile for 'bag' module
#
# Alex Wolf, July 2016

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -lm 
PROG = bagtest
OBJS = bagtest.c bag.c

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

bagtest.o: bag.h
bag.o: bag.h

clean:
	rm -f *~
	rm -f *.o
	rm -rf $(PROG).dSYM
	rm -f $(PROG)