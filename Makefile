CC=gcc
FLAGS=-std=c11

LIBOUT=libfarkle.a
EXECOUT=farkle

ifdef DEBUG
FLAGS+=-g -O0
endif

INCLUDE=-I . -L .
CFLAGS=$(FLAGS) $(INCLUDE)
LIB=-l farkle

ifdef FPIC
FLAGS+=-fPIC
endif

farkle: lib
	$(CC) $(CFLAGS) farkle.c $(LIB) -o $(EXECOUT)

lib: libfarkle.o
	ar rcs $(LIBOUT) libfarkle.o

libfarkle.o:
	$(CC) -c $(FLAGS) libfarkle.c

clean:
	rm -f $(LIBOUT) $(EXECOUT) *.o
