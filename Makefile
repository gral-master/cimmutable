CC = clang
CFLAGS = -Wall -Wextra -std=gnu11
LDFLAGS = -lcheck

OBJ = avl.o finger.o rrb_vector.o

all: lib
	mkdir -p bin
	mkdir -p lib

lib: compile $(OBJ)
	ar rcs lib/libimc.a $(OBJ)

compile:
	cd src: make -k

clean:
	rm -rf bin

mrproper:
	rm -rf bin lib
