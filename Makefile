CC = gcc
CFLAGS =  -ansi -Wall -pedantic
LDFLAGS = -lm  # Add this line to link the math library

all: program

program: main.o utill.o macoprocess.o
	$(CC) $(CFLAGS) -o program main.o utill.o macoprocess.o $(LDFLAGS)  # Add $(LDFLAGS) here

main.o: main.c asmbler.h
	$(CC) $(CFLAGS) -c main.c -o main.o

utill.o: utill.c asmbler.h
	$(CC) $(CFLAGS) -c utill.c -o utill.o

macoprocess.o: macoprocess.c asmbler.h
	$(CC) $(CFLAGS) -c macoprocess.c -o macoprocess.o

clean:
	rm -f main.o utill.o macoprocess.o program

.PHONY: all clean
