CC = gcc
RM = rm -f
ECHO = echo -e

CFLAGS = -Wall -Wextra -Wimplicit -std=gnu11 -g -fPIC

SRC := imc_avl.c imc_avl_map.c imc_avl_vector.c
OBJ = $(SRC:%.c=%.o)

all: $(OBJ) mv

$(OBJ): %.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@ 2>>log
	@$(ECHO) "\e[34mCompiled "$<" successfully!\e[0m"

mv:
	@mv *.o ../../obj/
