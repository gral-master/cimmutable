CC=gcc
CFLAGS=-c -Wall -g
BDIR=bin
ODIR=obj
SRC:=$(wildcard $(SDIR)/*.c)
OBJS:=$(SRC:%.c=%.o)
MKDIR=mkdir -p

all: clean cfingertree

cfingertree: $(ODIR)/main.o $(ODIR)/fingertree.o ../debug.o
	$(MKDIR) $(BDIR)
	$(CC) -o $(BDIR)/test $(ODIR)/main.o $(ODIR)/fingertree.o

$(ODIR)/%.o: %.c
	$(MKDIR) $(ODIR)
	$(CC) -o $@ $(CFLAGS) $<

../debug.o: ../debug.c
	$(CC) -o $@ $(CFLAGS) $<

clean:
	rm -f $(BDIR)/* $(ODIR)/* ../debug.o
