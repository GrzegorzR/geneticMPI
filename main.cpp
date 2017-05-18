#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <stdlib.h>
#include <mpi.h>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/set.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include "./genetic_logic.hpp"
#include "./parallel_logic.hpp"


using namespace std;





int main(int argc,char** argv)
{
    //test

    int rank, size, i;
    int buffer[10];
    MPI_Status status;
 
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);



   
    tuples_org = createSolutionTuples();
    vector<Solution *> population = createPopulation(INITIAL_SOLUTIONS_AMOUNT);
    //debug
    // printPopulation(population);
    // debug
   

    if (rank == 0){
        SerializedPopulation sp = serilize(population);
        int sum_of_elems = 0;

        broadcastPopulation(sp,MPI_COMM_WORLD);
        //MPI_Bcast(&sp.tuplesNumInPeriods[0], 250, MPI_INT, 0, MPI_COMM_WORLD);
        //MPI_Send(&sp.tuplesNumInPeriods[0], periodsNum, MPI_INT, 1, 123, MPI_COMM_WORLD);

    }
    printPopulation(population);
    cout << endl << endl;
    if (rank == 1)
    {

        SerializedPopulation sp = recivePopulation( MPI_COMM_WORLD, &status);
        vector<Solution *> population = deserialize(printPopulation);
        sp(population);

 /*       int recived[250];
        //MPI_Recv(recived, 250, MPI_INT, 0, 123, MPI_COMM_WORLD, &status);
        MPI_Bcast(recived, 250, MPI_INT, 0, MPI_COMM_WORLD);

        for (i=0; i<10; i++)
        {
            cout << recived[i] << endl; 
        }
        cout << endl;*/
        cout << rank << endl;
    }
    MPI_Finalize();
    return 0;

/*
    for(std::vector<int>::iterator it = sp.tuplesNumInPeriods.begin(); it != sp.tuplesNumInPeriods.end(); ++it)
        sum_of_elems += *it;

    cout << sum_of_elems << " " <<  sp.tuplesIds.size() << endl;*/
    // epoch iterator

}
