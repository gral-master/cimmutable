CC = clang
CFLAGS = -Wall -Wextra -std=gnu11

executable: compile

compile:
	@cd parser; make -k
	@cd avl; make -k $(MAKECMDGOALS)
	@cd finger; make -k $(MAKECMDGOALS)
	@cd rrb_vector; make -k $(MAKECMDGOALS)
