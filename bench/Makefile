CC = gcc
RM = rm -f
ECHO = echo -e

CFLAGS = -Wall -Wextra -Wimplicit -std=gnu11 -g -fPIC

SRC := bench_main.c parser.c
OBJ = $(SRC:%.c=%.o)

all: $(OBJ) mv

$(OBJ): %.o : %.c
	@$(CC) $(CFLAGS) -c $< -o $@
	@$(ECHO) "\e[32mCompiled "$<" successfully!\e[0m"

mv:
	@mv *.o ../obj
