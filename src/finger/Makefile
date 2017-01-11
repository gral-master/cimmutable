.PHONY: all clean

HSD=hs_ref

CC=gcc
CFLAGS=-g --std=c11 -Wall -Wextra

all: fingers

%.o: %.c
	$(CC) $(CFLAGS) -c $<

fingers: fingers.o
	$(CC) $(CFLAGS) fingers.o -o fingers

hs-example: $(HSD)/haskell_impl.hs
	ghc -XOverloadedLists -XTypeFamilies  $(HSD)/haskell_impl.hs && $(HSD)/haskell_impl

clean:
	rm -rf *.o a.out
