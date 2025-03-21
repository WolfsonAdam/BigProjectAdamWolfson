CC = gcc
CFLAGS = -ansi -Wall -pedantic -g
LDFLAGS = -lm

OBJS = main.o utill.o macoprocess.o first_pass.o second_pass.o
TARGET = assembler

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

# Generic rule for compiling .c to .o
%.o: %.c asmbler.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean