#!/bin/bash

make send_recv reduce isend_irecv > /dev/null

if [ $? -ne 0 ] ; then
    echo "Please install openmpi"
fi

for p in send_recv reduce isend_irecv ; do
    echo "Benchmarking $p..."
    for i in {1..10} ; do
        time mpiexec -n $i ./$p 0 10 1000
    done
    echo -e "\n\n"
done

make clean > /dev/null
