CC=mpicc
FLAGS=-g -Wall
BINS=monte_mpi monte_omp

all: $(BINS)

monte_%: monte_%.o monte_util.o
	$(CC) $^ -o $@ $(FLAGS)

%.o: %.c
	$(CC) -c $< $(FLAGS)

clean:
	rm -rf monte_mpi monte_omp *.o

