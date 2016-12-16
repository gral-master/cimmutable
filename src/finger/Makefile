.PHONY: all clean

CC=gcc
CFLAGS=-Wall -Wextra

all: finger

finger: fingers.c types.h
	$(CC) $(CFLAGS) fingers.c -o fingers

clean:
	rm -rf *.o
