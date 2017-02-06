CC = gcc
RM = rm -f
ECHO = echo -e

CFLAGS = -Wall -Wextra -Wimplicit -std=gnu11 -g -fPIC

SRC := rrb_checker.c rrb_vect.c vector.c
OBJ = $(SRC:%.c=%.o)

all: $(OBJ) mv

$(OBJ): %.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@ 2>>log
	@$(ECHO) "\e[34mCompiled "$<" successfully!\e[0m"
mv:
	@mv *.o ../../obj/

