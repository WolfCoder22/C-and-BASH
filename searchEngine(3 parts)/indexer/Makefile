# Makefile TEMPLATE for 'indexer' module
#
# David Kotz, April 2016
# updated by Xia Zhou, Alex Wolf July 2016

OBJS1 = indexer.c
OBJS2 = indextest.c
OBJS3= indextestShell
LLIBS = ../common/TSElib.a ../lib/cs50ds.a

CFLAGS = -Wall -pedantic -std=c11 -ggdb
CC = gcc
MAKE = make

# build the crawler
all: indexer indextest indextestShell

indexer: $(OBJS1) $(LLIBS)
	$(CC) $(CFLAGS) $^  -o $@

indextest: $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $^  -o $@

indextestShell: 
	chmod +x $(OBJS3)

# indexersource dependencies; add others as needed
indexer.o: indexer.h ../common/index.h ../common/word.h 

indextest.o: ../common/index.h ../common/file.h 

# build the library
../lib/cs50ds.a: 
	@cd ../lib; $(MAKE)

../common/TSElib.a: 
	@cd ../common; $(MAKE)

.PHONY: clean
	
clean:
	rm -f *~
	rm -f *.o
	@cd ../lib; $(MAKE) clean
	@cd ../common; $(MAKE) clean

