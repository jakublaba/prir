CC=mpicc
BINS=diss pipe
FLAGS=-g -Wall -lm

all: $(BINS)

diss: diss.o util.o
	$(CC) $^ -o $@ $(FLAGS)

pipe: pipe.o util.o
	$(CC) $^ -o $@ $(FLAGS)

%.o: %.c
	$(CC) -c $< $(FLAGS)

clean:
	rm -rf $(BINS) *.o

