CC = clang
CFLAGS = -Wall -Wextra -std=gnu11
LDFLAGS = -Iinclude

EXEC_TESTS = rrb_tests avl_tests finger_tests
SRC_TESTS = $(addsuffix .c, $(EXEC_TESTS))
LDTESTS = -lcheck -Iinclude -limc -Llib
OBJ = $(addprefix bin/, avl.o finger.o rrb_vector.o)

all: lib

lib: compile $(OBJ)
	ar rcs lib/libimc.a $(OBJ)

compile:
	mkdir -p bin
	mkdir -p lib
	cd src; make -k

tests: lib compile_tests

compile_tests: $(addprefix tests/, $(SRC_TESTS))
	for obj in $(basename $(SRC_TESTS)) ; do \
		$(CC) $(CFLAGS) $(LDTESTS) -o $(addprefix bin/, $$obj) tests/$$obj.c; \
	done

clean:
	rm -rf bin

mrproper:
	rm -rf bin lib

.PHONY: all lib compile tests clean mrproper
