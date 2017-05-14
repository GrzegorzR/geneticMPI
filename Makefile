CC      = g++
CFLAGS  = -lm -std=gnu++11
LDFLAGS = -lfltk

all: mpi

simple:
	g++ $(CFLAGS) main.cpp

mpi:
	mpicxx -o a.out $(CFLAGS)  main.cpp 

run:
	mpirun -np 2 ./a.out

clean:
	rm *.out