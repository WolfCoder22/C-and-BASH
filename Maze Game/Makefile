# Makefile TEMPLATE for 'crawler' module, updated for 'AMStartup' module
#
# David Kotz, April 2016
# updated by Xia Zhou, July 2016
# updated by Galen Brown, Aug 2016

PROG = AMStartup
OBJS = AMStartup.c Functions.c GUI.c


CFLAGS = -Wall -pedantic -std=c11 -ggdb
GFLAGS=`pkg-config --cflags --libs gtk+-3.0`

CC = gcc
MAKE = make

# build targets


$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $(GFLAGS) $^ -o $@


Functions.o: amazing.h Functions.h
AMStartup.o: amazing.h Functions.h GUI.h

.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)