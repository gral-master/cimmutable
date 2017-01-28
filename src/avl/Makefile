CC=gcc
CFLAGS=-W -Wall -std=gnu11 -pedantic -O0 -g
LDFLAGS=
EXEC= vector map bench
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

vector: vector_main.o avl.o avl_vector.o
	@$(CC) -o $@ $^ $(LDFLAGS)

map: map_main.o avl.o avl_map.o
	@$(CC) -o $@ $^ $(LDFLAGS)

bench: bench_main.o avl_map.o avl_vector.o avl.o parser.o
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
