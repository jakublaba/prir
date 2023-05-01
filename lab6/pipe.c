#include <mpi.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VEC_L 1000000
#define PART_L 1000

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int size, rank;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  MPI_Comm comm;
  int dims[] = {size};
  int periods[] = {false};
  MPI_Cart_create(MPI_COMM_WORLD, 1, dims, periods, true, &comm);
  int comm_rank;
  MPI_Comm_rank(comm, &comm_rank);

  int prev_rank, next_rank;
  MPI_Cart_shift(comm, 0, 1, &prev_rank, &next_rank);

  int *v;
  if (comm_rank == 0) {
    v = calloc(VEC_L, sizeof(int));
  }

  long sum = 0;
  for (int i = 0; i < VEC_L / PART_L; ++i) {
    int part[PART_L];
    if (comm_rank == 0) {
      int offset = i * PART_L;
      memcpy(part, v + offset, PART_L * sizeof(int));
    }

    MPI_Recv(part, PART_L, MPI_INT, prev_rank, 0, comm, MPI_STATUS_IGNORE);

    if (comm_rank != 0 && comm_rank != size - 1) {
      for (int i = 0; i < PART_L; ++i) {
        part[i] = part[i] + 1;
      }
    }

    if (comm_rank == size - 1) {
      for (int i = 0; i < PART_L; ++i) {
        sum += part[i];
      }
    }

    MPI_Send(part, PART_L, MPI_INT, next_rank, 0, comm);
  }

  if (comm_rank == size - 1) {
    printf("Sum = %ld\n", sum);
  }

  MPI_Finalize();
  return EXIT_SUCCESS;
}

