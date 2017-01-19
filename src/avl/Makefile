CC=gcc
CFLAGS=-W -Wall -std=c99 -pedantic -O0 -g
LDFLAGS=
EXEC= avl vector
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

avl: main.o avl.o
	@$(CC) -o $@ $^ $(LDFLAGS)

vector: vector_main.o avl.o avl_vector.o
	@$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
