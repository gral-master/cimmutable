CC = clang
CFLAGS = -Wall -Wextra -std=gnu11
LDFLAGS = -Iinclude

LDTESTS = -lcheck -Iinclude -limc -Llib
OBJ = $(addprefix bin/, avl_map.o avl_vector.o avl.o fingers.o parser.o rrb_dumper.o rrb_vector.o tools.o vector.o)

all: lib

lib: compile $(OBJ)
	ar rcs lib/libimc.a $(OBJ)

executable: compile

compile:
	@mkdir -p bin
	@mkdir -p lib
	@mkdir -p exe
	@cd src; make -k $(MAKECMDGOALS)

clean:
	rm -rf bin

mrproper:
	rm -rf bin lib exe

.PHONY: all lib compile tests clean mrproper
