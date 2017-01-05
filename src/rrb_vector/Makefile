.PHONY: all clean launch

CC=gcc
CFLAGS=-Wall -Wextra -std=gnu11 -g

all: launch

launch: rrb rrb_vector.o rrb_dumper.o main.o
	@./rrb
	@dot -Tps rrb-tree.dot -o rrb-tree.svg

rrb: rrb_vector.o rrb_dumper.o main.o
	$(CC) $(CFLAGS) rrb_vector.o rrb_dumper.o main.o -o rrb

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf *.o
