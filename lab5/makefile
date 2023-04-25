CC=mpicc
FLAGS=-g

send_recv: send_recv.o
	$(CC) -o send_recv send_recv.o $(FLAGS)

send_recv.o: send_recv.c
	$(CC) -c send_recv.c $(FLAGS)

scatter_reduce: scatter_reduce.o
	$(CC) -o scatter_reduce scatter_reduce.o $(FLAGS)

scatter_reduce.o: scatter_reduce.c
	$(CC) -c scatter_reduce.c $(FLAGS)

isend_irecv: isend_irecv.o
	$(CC) -o isend_irecv isend_irecv.o $(FLAGS)

isend_irecv.o: isend_irecv.c
	$(CC) -c isend_irecv.c $(FLAGS)

clean:
	rm -rf send_recv scatter_reduce isend_irecv *.o
