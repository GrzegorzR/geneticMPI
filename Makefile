CC      = g++
CFLAGS  = -lm -std=gnu++11 -lboost_serialization
LDFLAGS = -lfltk

all: mpi

simple:
	g++ $(CFLAGS) main.cpp

mpi:
	mpicxx -o a.out $(CFLAGS)  main.cpp 

run:
	mpirun -np 8 ./a.out

clean:
	rm *.out