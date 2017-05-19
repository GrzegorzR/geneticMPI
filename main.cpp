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

        broadcastPopulation(sp);
    printPopulation(population);


    }
    //cout << endl << "----------------------------------"<< rank<<endl;
    if (rank == 1){

        SerializedPopulation sp = recivePopulation( 0, &status);
        vector<Solution *> pop = deserialize(sp);
        printPopulation(pop);

        //vector<Solution *> newSolutions = createNewSolutions(vector<Solution *> population);
        //SerializedPopulation newSolSerialized = serilize(newSolutions);
        //sendNewSolutionsToMaster(newSolSerialized);

    }


    MPI_Finalize();
    return 0;



}
