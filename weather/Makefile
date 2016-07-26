CC=gcc
CFLAGS=-Wall -pedantic -std=c11  -lm -lcurl
OBJ=weather.c mycurl.c

weather: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

mycurl: $(OBJ)
	gcc -o $@ $^ $(CFLAGS)

clean:
	rm -f *.o $(objects) 