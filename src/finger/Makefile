CC=gcc
CFLAGS=-c -Wall
BDIR=bin
ODIR=obj
OBJS=main.o fingertree.o
MKDIR=mkdir -p

all: cfingertree

cfingertree: $(ODIR)/main.o $(ODIR)/fingertree.o
	$(MKDIR) $(BDIR)
	$(CC) -o $(BDIR)/test $(ODIR)/main.o $(ODIR)/fingertree.o
    
$(ODIR)/%.o: %.c
	$(MKDIR) $(ODIR)
	$(CC) -o $@ $(CFLAGS) $<
    
clean:
	rm -f $(BDIR)/* $(ODIR)/*
