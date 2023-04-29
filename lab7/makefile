CC=mpicc
FLAGS=-g -Wall
BINS=monte_mpi monte_omp

all: $(BINS)

monte_mpi: monte_mpi.o monte_util.o
	$(CC) $^ -o $@ $(FLAGS)

monte_omp: monte_omp.o monte_util.o
	$(CC) $^ -o $@ $(FLAGS) -fopenmp

monte_omp.o: monte_omp.c
	$(CC) -c $< -o $@ $(FLAGS) -fopenmp

%.o: %.c
	$(CC) -c $< $(FLAGS)

clean:
	rm -rf monte_mpi monte_omp *.o

