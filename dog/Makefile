CC=gcc
CFLAGS= -Wall -pedantic -std=c11 -ggdb
OBJ=dog.c

dog: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)
clean:
	rm -f *.o $(objects) dog