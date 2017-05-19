#include <iostream>
#include <random>
#include <vector>
#include <set>
#include <algorithm>
#include <functional>
#include <stdlib.h>
#include <climits>
#include <fstream>
#include <mpi.h>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/set.hpp>
#include <boost/iostreams/stream_buffer.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include "./genetic_logic.hpp"
#include "./parallel_logic.hpp"
#include <time.h>       /* time */


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

    srand(time(NULL) + rank);


    if (rank == 0){
        SerializedPopulation sp = serilize(population);
        int sum_of_elems = 0;

        broadcastPopulation(sp,MPI_COMM_WORLD);


    }
   // printPopulation(population);
    cout << endl << endl;
    if (rank != 0)
    {

        SerializedPopulation sp = recivePopulation( MPI_COMM_WORLD, &status);
        vector<Solution *> population = deserialize(sp);
        vector<Solution *> newSolutions = createNewSolutions(vector<Solution *> population);
        SerializedPopulation newSolSerialized = serilize(newSolutions);
        sendNewSolutionsToMaster

        //sp(population);

    }
    MPI_Finalize();
    return 0;

/*
    for(std::vector<int>::iterator it = sp.tuplesNumInPeriods.begin(); it != sp.tuplesNumInPeriods.end(); ++it)
        sum_of_elems += *it;

    cout << sum_of_elems << " " <<  sp.tuplesIds.size() << endl;*/
    // epoch iterator

}
