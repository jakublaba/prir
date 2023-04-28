CC=mpicc
FLAGS=-g

monte_mpi: monte_mpi.o
	$(CC) -o monte_mpi monte_mpi.o $(FLAGS)

monte_mpi.o: monte_mpi.c
	$(CC) -c monte_mpi.c $(FLAGS)

monte_omp: monte_omp.o
	$(CC) -o monte_omp monte_omp.o $(FLAGS)

monte_omp.o: monte_omp.c
	$(CC) -c monte_omp.c $(FLAGS)

clean:
	rm -rf monte_mpi monte_omp *.o
