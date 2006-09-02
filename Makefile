# Yueq C Compiler
# Author : K.C.L
# Further Modified by Qi Yue (yueq@cse.buaa.edu.cn)

CC = gcc

CFLAGS = 

OBJS = main.o util.o scan.o tree.o symtab.o semantic.o code.o generate.o

ycc: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o ycc

main.o: main.c globals.h util.h scan.h tree.h semantic.h generate.h
	$(CC) $(CFLAGS) -c main.c

util.o: util.c util.h globals.h
	$(CC) $(CFLAGS) -c util.c

scan.o: scan.c scan.h util.h globals.h
	$(CC) $(CFLAGS) -c scan.c

tree.o: tree.c tree.h scan.h globals.h util.h
	$(CC) $(CFLAGS) -c tree.c

symtab.o: symtab.c symtab.h
	$(CC) $(CFLAGS) -c symtab.c

semantic.o: semantic.c globals.h symtab.h semantic.h
	$(CC) $(CFLAGS) -c semantic.c

code.o: code.c code.h globals.h
	$(CC) $(CFLAGS) -c code.c

generate.o: generate.c globals.h symtab.h code.h generate.h
	$(CC) $(CFLAGS) -c generate.c

clean:
	-rm ycc
	-rm yvm
	-rm $(OBJS)

yvm: yvm.c
	$(CC) $(CFLAGS) yvm.c -o yvm

all: ycc yvm

