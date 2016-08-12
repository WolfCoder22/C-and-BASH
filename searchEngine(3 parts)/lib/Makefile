# Makefile TEMPLATE for 'cs50ds' library
#   Builds and compiles all the data structures into a library.
#
# David Kotz, April 2016
# updated by Xia Zhou and Alex Wolf, July 2016

OBJS = bag/bag.o set/set.o counters/counters.o hashtable/hashtable.o hashtable/jhash.o memory/memory.o
LIB = cs50ds.a
CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = @gcc
MAKE = make
MAKEC= make clean

$(LIB): $(OBJS)
	@ar cr $@ $^

# add other targets for the other .o files

memory/memory.o: memory/memory.c memory/memory.h
	@cd memory; $(MAKE) memory.o

bag/bag.o: bag/bag.c bag/bag.h
	@cd bag; $(MAKE) bag.o

set/set.o: set/set.c set/set.h
	@cd set; $(MAKE) set.o

hashtable/jhash.o: hashtable/jhash.c hashtable/jhash.h	
	@cd hashtable; $(MAKE) jhash.o

hashtable/hashtable.o: hashtable/hashtable.c hashtable/hashtable.h hashtable/jhash.c hashtable/jhash.h set/set.c set/set.h
	@cd hashtable; $(MAKE) hashtable.o

# add cleanup commands for other subdirectories
.PHONY: clean
	
clean:
	@rm -f *~
	@rm -f *.o
	@rm -f $(LIB)
	@cd memory; $(MAKEC) 
	@cd bag; $(MAKEC) 
	@cd set; $(MAKEC) 
	@cd hashtable; $(MAKEC) 
	@cd counters; $(MAKEC) 
