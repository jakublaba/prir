#!/bin/bash

make > /dev/null

echo "Benchmarking OpenMP version..."
for i in 10, 100, 1000, 10000, 100000 ; do
  time ./monte_omp $i
done

echo "Benchmarking OpenMPI version..."
for i in 10, 100, 1000, 10000, 100000 ; do
  for n in {1..2} ; do
    time mpiexec -n $n ./monte_mpi $i
  done
done

make clean > /dev/null

