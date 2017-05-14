CC      = g++
CFLAGS  = -lm -std=gnu++11 
LDFLAGS = -lboost_serialization

all: mpi

simple:
	g++ $(CFLAGS) main.cpp

mpi:
	mpicxx -o a.out $(CFLAGS)  main.cpp $(LDFLAGS)

run:
	mpirun -np 8 ./a.out

clean:
	rm *.out