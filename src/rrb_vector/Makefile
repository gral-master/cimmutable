CC = clang
CFLAGS = -Wall -Wextra -std=gnu11 -O3
EXEC = $(addprefix ../../exe/, rrb)
SRC = rrb_vector.c rrb_dumper.c parser.c
OBJ = $(addprefix ../../bin/, $(SRC:%.c=%.o))


all: $(OBJ)

executable : $(EXEC)

../../exe/rrb: $(OBJ) ../../bin/rrb_bench.o
	$(CC) $(CFLAGS) $^ -o $@

../../bin/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all clean launch
