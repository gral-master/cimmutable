CC = gcc
CFLAGS = -W -Wall -std=gnu11 -pedantic -O3
LDFLAGS = -lm
EXEC = $(addprefix ../../exe/, avl_vector avl_map avl_bench)
SRC = $(wildcard *.c)
OBJ = $(addprefix ../../bin/, $(SRC:.c=.o))

all: $(OBJ)

executable: $(EXEC)

../../exe/avl_vector: $(addprefix ../../bin/, vector_main.o avl.o avl_vector.o)
	@$(CC) -o $@ $^ $(LDFLAGS)

../../exe/avl_map: $(addprefix ../../bin/, map_main.o avl.o avl_map.o)
	@$(CC) -o $@ $^ $(LDFLAGS)

../../exe/avl_bench: $(addprefix ../../bin/, bench_main.o avl_map.o avl_vector.o avl.o parser.o)
	@$(CC) -o $@ $^ $(LDFLAGS)

../../bin/%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

.PHONY: all executable
