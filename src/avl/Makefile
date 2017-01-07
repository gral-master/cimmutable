CC=gcc
CFLAGS=-W -Wall -std=c99 -pedantic -O3
LDFLAGS=
EXEC= avl
SRC= $(wildcard *.c)
OBJ= $(SRC:.c=.o)

all: $(EXEC)

avl: main.o avl.o
	@$(CC) -o $@ $^ $(LDFLAGS)

main.o: avl.h avl.c

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: clean mrproper

clean:
	@rm -rf *.o

mrproper: clean
	@rm -rf $(EXEC)
