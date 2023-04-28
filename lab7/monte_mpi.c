#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <mpi.h>

#include "monte.h"

double rand_range(double low, double up) {
    double range = up - low;
    double div = RAND_MAX / range;
    
    return low + (rand() / div);
}

bool f(point_t* pt) {
    return pt->x*pt->x + pt->y*pt->y < 1;
}

point_t* new_point(double x, double y) {
    point_t* pt = malloc(sizeof(point_t));
    pt->x = x;
    pt->y = y;

    return pt;
}

point_t* rand_pt() {
    return new_point(rand_range(-1., 1.), rand_range(-1, 1.));
}

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
        double pi = global_count * 4. / n_pts;
        printf("PI ~= %f\n", pi);
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}
