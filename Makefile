CC=gcc
FLAGS=-std=c11

ifdef DEBUG
FLAGS+=-g -O0
endif

INCLUDE=-I . -L .
CFLAGS=$(FLAGS) $(INCLUDE)
LIB=-l farkle

farkle: lib
	$(CC) $(CFLAGS) farkle.c $(LIB) -o farkle

lib: libfarkle.o
	ar rcs libfarkle.a libfarkle.o

libfarkle.o:
	$(CC) -c $(FLAGS) libfarkle.c

clean:
	rm -f farkle *.o *.a
