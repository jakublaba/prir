#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <mpi.h>

#include "monte.h"

int main(int argc, char** argv) {
    srand(time(NULL));

    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (argc != 2) {
        fprintf(stderr, "Usage: %s n_pts\n", argv[0]);
        return EXIT_FAILURE;
    }

    int n_pts = atoi(argv[1]);
    int local_n_pts = n_pts / size;
    int global_count = 0;

    // best load balancing on this hemisphere
    if (rank == size - 1) {
        local_n_pts += n_pts % size;
    }

    int local_count = 0;
    for (int i = 0; i < n_pts; ++i) {
        point_t* rpt = rand_pt();
        if (f(rpt)) {
            local_count++;
        }
        free(rpt);
    }

    MPI_Reduce(&local_count, &global_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0){
        double pi = global_count * 4. / n_pts / size;
        printf("PI ~= %f\n", pi);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
