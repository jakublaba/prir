#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

double f(double x) {
    return x * x / 3;
}

double integrate(double a, double b, int n, double (*f)(double)) {
    double h = (b - a) / n;
    double sum = (f(a) + f(b)) / 2.;
    for (int i = 1; i < n; ++i) {
        sum += f(a + i * h);
    }

    return sum * h;
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 4) {
        fprintf(stderr, "Usage: %s a b n\n", argv[0]);
        fprintf(stderr, "Where:\na-b - integration interval\nn - number of subintervals\n");
        MPI_Finalize();
        
        return EXIT_FAILURE;
    }

    double a = atof(argv[1]);
    double b = atof(argv[2]);
    int n = atoi(argv[3]);
    double h = (b - a) / n;

    int local_n = n / size;
    if (rank == size - 1) {
        local_n += n % size;
    }
    int local_a = a + rank * local_n * h;
    int local_b = local_a + local_n * h;
    double local_res = integrate(local_a, local_b, local_n, f);

    if (rank == 0) {
        double global_res = local_res;
        for (int i = 1; i < size; ++i) {
            double recv_res;
            MPI_Recv(&recv_res, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_res += recv_res;
        }
        printf("Result: %lf [%d procs]\n", global_res, size);
    } else {
        MPI_Send(&local_res, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
