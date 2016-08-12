# Makefile TEMPLATE for 'querier' module
#
# Alex Wolf, August 2016

PROG = querier
OBJS = querier.c  
LIBS = -lcurl
LLIBS = ../common/TSElib.a ../lib/cs50ds.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

# build the crawler
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@

# querier source dependencies; add others as needed
querier.c: querier.h 

# build the library
../common/TSElib.a: 
	@cd ../common; $(MAKE)

../lib/cs50ds.a: 
	@cd ../lib; $(MAKE)

.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	rm -f $(PROG)
	cd ../lib; $(MAKE) clean
	cd ../common; $(MAKE) clean
