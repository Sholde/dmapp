BIN=mpi openmp pthread


all:$(BIN)

mpi:mpi.c
	mpicc $^ -o $@

pthread:pthread.c
	gcc $^ -o $@ -lpthread

openmp:openmp.c
	gcc $^ -o $@ -fopenmp

test:$(BIN)
	./openmp
	./pthread
	mpirun -np 4 ./mpi

clean:
	rm $(BIN)
