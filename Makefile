# Makefile placeholder for shared library
# From : https://gist.github.com/xuhdev/1873316
# TODO finish it

LIBNAME = cimmutable.a

UTILDIR = ./src
OBJDIR = ./obj
#AVLDIR = ./src/avl
#FINGERDIR = ./src/finger
RRBDIR = ./src/rrb_vector
BENCHDIR = ./bench

B_FILE := ./bench/tests/202_int_vec.bench

CC = gcc

CFLAGS = -Wall -Wextra -Wimplicit -std=gnu11 -g -fPIC

SHELL = /bin/sh

RM = rm -f
ECHO = echo -e

all: clean $(LIBNAME)

$(LIBNAME): comp build_rrb
	@ar -cvq $@ $(OBJDIR)/*.o
	@$(ECHO) "\e[32mLibrary "$@" linked!\e[0m"

bench: comp build_rrb build_bench
	@$(CC) $(CFLAGS) $(OBJDIR)/*.o -lm

exec: bench
	./a.out -f $(B_FILE) -b

comp:
	@$(CC) $(CFLAGS) -c ./src/debug.c -o ./obj/debug.o
	@$(ECHO) "\e[34mCompiled debug.c successfully!\e[0m"

build_avl:
	@cd $(AVLDIR) && $(MAKE)

build_finger:
	@cd $(FINGERDIR) && $(MAKE)

build_rrb:
	@cd $(RRBDIR) && $(MAKE)

build_bench:
	@cd $(BENCHDIR) && $(MAKE)

clean:
	@$(RM) -v $(LIBNAME)
	@$(RM) -v ./a.out
	@find ./src -type f -iname '*.o' -delete
	@$(RM) -v $(OBJDIR)/*.o 
