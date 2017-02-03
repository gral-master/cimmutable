CC=gcc
CFLAGS=-c -Wall -g -std=c11 -O3
BDIR=bin
ODIR=obj
SRC:=$(wildcard $(SDIR)/*.c)
OBJS:=$(SRC:%.c=%.o)
MKDIR=mkdir -p

all: clean cfingertree

run: all
	./bin/test

memtest: all
	valgrind --leak-check=yes --show-reachable=yes --tool=memcheck ./bin/test

cfingertree: $(ODIR)/main.o $(ODIR)/fingertree.o ../debug.o $(ODIR)/list.o $(ODIR)/invariants.o $(ODIR)/reflist.o $(ODIR)/imc_ft_vector.o $(ODIR)/imc_ft_map.o
	$(MKDIR) $(BDIR)
	$(CC) -o $(BDIR)/test $(ODIR)/main.o $(ODIR)/fingertree.o $(ODIR)/list.o $(ODIR)/invariants.o $(ODIR)/reflist.o $(ODIR)/imc_ft_vector.o $(ODIR)/imc_ft_map.o

bench: $(ODIR)/bench_main.o $(ODIR)/parser.o $(ODIR)/fingertree.o ../debug.o $(ODIR)/list.o $(ODIR)/invariants.o $(ODIR)/reflist.o $(ODIR)/imc_ft_vector.o $(ODIR)/imc_ft_map.o
	$(MKDIR) $(BDIR)
	$(CC) -o $(BDIR)/bench $(ODIR)/bench_main.o $(ODIR)/parser.o $(ODIR)/fingertree.o $(ODIR)/list.o $(ODIR)/invariants.o $(ODIR)/reflist.o $(ODIR)/imc_ft_vector.o $(ODIR)/imc_ft_map.o

$(ODIR)/%.o: %.c
	$(MKDIR) $(ODIR)
	$(CC) -o $@ $(CFLAGS) $<

../debug.o: ../debug.c
	$(CC) -o $@ $(CFLAGS) $<

clean:
	rm -f $(BDIR)/* $(ODIR)/* ../debug.o
