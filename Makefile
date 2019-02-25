CFLAGS+=-std=c99 
LDLIBS+=-lc

CFLAGS+=-fsanitize=address
LDLIBS+=-lasan

brainfuck: brainfuck.c brainfuck.h
clean:
	-rm brainfuck
