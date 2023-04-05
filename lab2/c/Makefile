CC=gcc
FLAGS=-g -pthread -lm

mat: matrix.o
	$(CC) -o mat matrix.o $(FLAGS)

matrix.o: matrix.c
	$(CC) -o matrix.o -c matrix.c $(FLAGS)

clean:
	rm -f mat *.o
