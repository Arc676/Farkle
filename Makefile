CC=gcc
FLAGS=-c

farkle: lib
	$(CC) -I . -L . farkle.c -l farkle -o farkle

lib: libfarkle.o
	ar rcs libfarkle.a libfarkle.o

libfarkle.o:
	$(CC) $(FLAGS) libfarkle.c

clean:
	rm -f farkle *.o *.a
