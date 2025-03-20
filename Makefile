CC = gcc
CFLAGS = -ansi -Wall -pedantic -g
LDFLAGS = -lm  # Add this line to link the math library
First_pass = first_pass.o

all: program

program: main.o utill.o macoprocess.o $(First_pass)
	$(CC) $(CFLAGS) -o program main.o utill.o macoprocess.o $(First_pass) $(LDFLAGS)  # Add $(LDFLAGS) here

main.o: main.c asmbler.h
	$(CC) $(CFLAGS) -c main.c -o main.o

utill.o: utill.c asmbler.h
	$(CC) $(CFLAGS) -c utill.c -o utill.o

macoprocess.o: macoprocess.c asmbler.h
	$(CC) $(CFLAGS) -c macoprocess.c -o macoprocess.o

first_pass.o: first_pass.c asmbler.h
	$(CC) $(CFLAGS) -c first_pass.c -o first_pass.o
	
first_pass.o: second_pass.c asmbler.h
	$(CC) $(CFLAGS) -c first_pass.c -o first_pass.o

clean:
	rm -f main.o utill.o macoprocess.o first_pass.o program

.PHONY: all clean
