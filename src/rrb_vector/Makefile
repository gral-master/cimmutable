.PHONY: all clean launch

SRC = rrb_vector.c rrb_dumper.c parser.c
OBJ = $(SRC:%.c=%.o)

CC = clang
CFLAGS = -Wall -Wextra -std=gnu11 -g

all: preparation launch #test

preparation:
	@mkdir -p bin/
	@mkdir -p exec/

test: exec/test
	@./exec/test

exec/test: bin/test.o $(addprefix bin/, $(OBJ))
	$(CC) $(CFLAGS) $^ -o $@

launch: exec/rrb
	@./exec/rrb

#@dot -Tps rrb-tree.dot -o rrb-tree.svg
exec/rrb: $(addprefix bin/, $(OBJ)) bin/rrb_bench.o
	$(CC) $(CFLAGS) $^ -o $@

bin/test.o: tests/test.c
	$(CC) $(CFLAGS) -c $< -o $@

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf bin/

mrproper:
	rm -rf exec/
	rm -rf bin/
