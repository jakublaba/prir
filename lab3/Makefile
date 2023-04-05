CC=gcc
FLAGS=-g -fopenmp -lm

prime: prime.o array.o
	$(CC) -o prime prime.o array.o $(FLAGS)

prime.o: prime.c
	$(CC) -c -o prime.o prime.c

array.o: array.c
	$(CC) -c -o array.o array.c

clean:
	rm -rf prime *.o

