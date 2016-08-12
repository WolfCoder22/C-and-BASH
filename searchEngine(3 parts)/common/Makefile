# Makefile TEMPLATE for 'common' library
#   Builds and compiles all the data structures into a library.
#
# David Kotz, April 2016
# updated by Xia Zhou and Alex Wolf, July 2016

OBJS = web.o file.o webpage.o word.o index.o

LIB = TSElib.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

$(LIB): $(OBJS)
	@ar cr $@ $^

# add other targets for the other .o files
web.o: web.h

file.o: file.c file.h

webpage.o:  webpage.h ../lib/memory/memory.h

word.o: word.c word.h

index.o: file.h index.h ../lib/memory/memory.h ../lib/hashtable/hashtable.h ../lib/counters/counters.h



# add cleanup commands for other subdirectories
.PHONY: clean
	
clean:
	@rm -f *~
	@rm -f *.o
	@rm -f $(LIB)
	@cd ../lib/memory; $(MAKE) $@
	@cd ../lib/hashtable; $(MAKE) $@
	@cd ../lib/counters; $(MAKE) $@


