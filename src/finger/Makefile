.PHONY: all clean

HSD=hs_ref

CC=gcc
CFLAGS=--std=c11 -Wall -Wextra

all: fingers.o

%.o: %.c
	$(CC) $(CFLAGS) -c $<

hs-example: $(HSD)/haskell_impl.hs
	ghc -XOverloadedLists -XTypeFamilies  $(HSD)/haskell_impl.hs && $(HSD)/haskell_impl

clean:
	rm -rf *.o a.out
