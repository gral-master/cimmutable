CC = gcc
CFLAGS = -g -std=gnu11 -Wall -Wextra
LDFLAGS =
EXEC = $(addprefix ../../exe/, fingers finger_bench)
SRC = $(wildcard *.c)
OBJ = $(addprefix ../../bin/, $(SRC:.c=.o))

all: $(OBJ)

executable: $(EXEC)

../../bin/%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)

../../exe/fingers: $(addprefix ../../bin/, finger_test.o fingers.o tools.o)
	$(CC) $(CFLAGS) $^ -o $@

../../exe/finger_bench: $(addprefix ../../bin/, bench_main.o vector.o fingers.o tools.o parser.o avl_map.o avl.o avl_vector.o)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: all clean
