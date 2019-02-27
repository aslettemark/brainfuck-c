CFLAGS+=-std=c99 -O2
LDLIBS+=-lc

#CFLAGS+=-fsanitize=address -g
#LDLIBS+=-lasan

brainfuck: brainfuck.c brainfuck.h

clean:
	-rm brainfuck
