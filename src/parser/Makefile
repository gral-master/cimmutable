CC = gcc
CFLAGS = -W -Wall -std=gnu11 -pedantic -O3
LDFLAGS = -lm
SRC = $(wildcard *.c)
OBJ = $(addprefix ../../bin/, $(SRC:.c=.o))

all: $(OBJ)

../../bin/%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: all
