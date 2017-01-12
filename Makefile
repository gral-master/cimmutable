# Makefile placeholder for shared library
# From : https://gist.github.com/xuhdev/1873316
# TODO finish it

LIBNAME = cimmutable.a

UTILDIR = ./src
OBJDIR = ./obj
#AVLDIR = ./src/avl
#FINGERDIR = ./src/finger
RRBDIR = ./src/rrb_vector

CC = gcc

CFLAGS = -Wall -Wextra -Wimplicit -std=gnu11 -g -fPIC

SHELL = /bin/sh

RM = rm -f
ECHO = echo -e

all: clean $(LIBNAME)

$(LIBNAME): comp build_rrb
	@ar -cvq $@ $(OBJDIR)/*.o
	@$(ECHO) "\e[32mLibrary "$@" linked!\e[0m"

comp:
	@$(CC) $(CFLAGS) -c ./src/debug.c -o ./obj/debug.o
	@$(ECHO) "\e[34mCompiled debug.c successfully!\e[0m"

build_avl:
	@cd $(AVLDIR) && $(MAKE)

build_finger:
	@cd $(FINGERDIR) && $(MAKE)

build_rrb:
	@cd $(RRBDIR) && $(MAKE)

clean:
	@$(RM) $(LIBNAME)
	@find ./src -type f -iname '*.o' -delete
	@$(RM) $(OBJDIR)/*.o 
