.PHONY: all clean launch

SRC = rrb_vector.c rrb_dumper.c main.c
OBJ = $(SRC:%.c=%.o)

CC = clang
CFLAGS = -Wall -Wextra -std=gnu11 -g

all: preparation launch

preparation:
	mkdir -p bin/
	mkdir -p exec/

launch: exec/rrb $(OBJ)
	@./exec/rrb
	# @dot -Tps rrb-tree.dot -o rrb-tree.svg

exec/rrb: $(addprefix bin/, $(OBJ))
	$(CC) $(CFLAGS) $^ -o $@

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf bin/

mrproper:
	rm -rf exec/
	rm -rf bin/
