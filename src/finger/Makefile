.PHONY: all clean

INCLUDES=tools.h fingers.h
HSD=hs_ref

CC=gcc
CFLAGS=-g --std=c11 -Wall -Wextra
LDFLAGS=

all: fingers

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

test: finger_test.o fingers.o tools.o
	$(CC) $(CFLAGS) finger_test.o fingers.o tools.o -o fingers

bench: bench_main.o vector.o fingers.o tools.o parser.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf *.o a.out
