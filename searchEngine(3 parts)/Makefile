# Makefile TEMPLATE for 'tse' library
#   Builds and compiles all subdirectories for the tse
#
# Alex Wolf July 2016

MAKE = make
MAKEC = make clean
DIRS = common crawler lib indexer

.PHONY: tse

tse:
	@cd common; $(MAKE) 
	@cd crawler; $(MAKE)
	@cd indexer; $(MAKE)
	@cd querier; $(MAKE)
	@cd lib; $(MAKE)

# add cleanup commands for other subdirectories
.PHONY: clean
	
clean:
	@cd lib; $(MAKEC)
	@cd common; $(MAKEC)
	@cd indexer; $(MAKEC)
	@cd querier; $(MAKEC)
	@cd crawler; $(MAKEC)
