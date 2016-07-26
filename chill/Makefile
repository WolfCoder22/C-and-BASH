CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -lm
OBJ=chill.c

chill: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
clean:
	rm -f *.o $(objects) chill