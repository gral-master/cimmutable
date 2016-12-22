.PHONY: all clean

CC=gcc
CFLAGS=-Wall -Wextra

all: vector

vector: fingers.o
	$(CC) $(CFLAGS) fingers.o vector.c

%.o: %.c
	$(CC) $(CFLAGS) $<

clean:
	rm -rf *.o
