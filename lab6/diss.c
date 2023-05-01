#include "util.h"
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  MPI_Init(NULL, NULL);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int nums[size];
  if (rank == 0 && argc - 1 != size) {
    fprintf(stderr, "Number of arguments must match number of procs\n");
    return EXIT_FAILURE;
  }
  for (int i = 0; i < argc - 1; ++i) {
    nums[i] = atoi(argv[i + 1]);
  }

  int steps = log2(size);
  int local_val = nums[rank];
  for (int i = 0; i < steps; ++i) {
    int offset = pow_i(2, i);
    int send_rank = (rank + offset) % size;
    int receive_rank = rank - offset;
    if (receive_rank < 0) {
      receive_rank += size;
    }
    int partner_val;
    MPI_Sendrecv(&local_val, 1, MPI_INT, send_rank, 0, &partner_val, 1, MPI_INT,
                 receive_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    local_val = gcd(local_val, partner_val);
  }
  if (rank == 0) {
    printf("GCD = %d\n", local_val);
  }
  MPI_Finalize();
  return EXIT_SUCCESS;
}
