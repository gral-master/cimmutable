# Makefile placeholder for shared library
# From : https://gist.github.com/xuhdev/1873316
# TODO finish it

LIBNAME = cimmutable.so

UTILDIR = ./src
OBJDIR = ./obj
INCLUDDIR = ./include
#AVLDIR = ./src/avl
#FINGERDIR = ./src/finger
RRBDIR = ./src/rrb_vector

SHELL = /bin/sh
CC = gcc
CFLAGS = -Wall -Wextra -Wimplicit -std=gnu11 -g -fPIC
LFLAGS = -shared

RM = rm -f
ECHO = echo -e

SRC := $(wildcard $(UTILDIR)/*.c) $(wildcard $(RRBDIR)/*.c)
INCLUDE := $(wildcard $(UTILDIR)/*.h) $(wildcard $(RRBDIR)/*.h)
INCLUDE += $(wildcard $(INCLUDDIR)/*.h)

OBJ := $(SRC:%.c=%.o)
ROBJ := $(foreach obj, $(notdir $(OBJ)), $(OBJDIR)/$(obj))


all: clean $(LIBNAME)

$(LIBNAME): $(ROBJ) $(INCLUDE)
	$(CC) $(LFLAGS) -o $@ $^
	@$(ECHO) "\e[32mLibrary "$@" linked!\e[0m"

$(OBJ): %.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(ECHO) "\e[34mCompiled "$<" successfully!\e[0m"

$(ROBJ): $(OBJ)
	@find ./src -type f -iname '*.o' -exec mv -t $(OBJDIR)/ {} \+

clean:
	@find ./src -type f -iname '*.o' -delete
	@$(RM) $(OBJDIR)/*.o 
