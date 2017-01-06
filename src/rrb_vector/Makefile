CC = gcc

CFLAGS = -Wall -Wextra -Wimplicit -std=gnu11 -g -fPIC


SRC := rrb_checker.c rrb_vect.c vector.c
OBJ = $(SRC:%.c=%.o)

all: $(OBJ)

$(OBJ): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f *.o
