.PHONY: all clean

INCLUDES=tools.h fingers.h
HSD=hs_ref

CC=gcc
CFLAGS=-g --std=c11 -Wall -Wextra

all: fingers

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

fingers: fingers.o tools.o
	$(CC) $(CFLAGS) fingers.o tools.o -o fingers

hs-example: $(HSD)/haskell_impl.hs
	ghc -XOverloadedLists -XTypeFamilies  $(HSD)/haskell_impl.hs && $(HSD)/haskell_impl

clean:
	rm -rf *.o a.out
