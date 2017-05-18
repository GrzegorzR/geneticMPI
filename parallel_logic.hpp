

struct SerializedPopulation{
    std::vector<int> tuplesNumInPeriods;
    std::vector<int> tuplesIds;
    std::vector<int> groupIds;
    std::vector<int> lecturerIds;
    std::vector<int> roomIds;
};

SerializedPopulation serilize(vector<Solution *> population){
    SerializedPopulation result;


    int *wektor = new int[PERIODS_AMOUNT*INITIAL_SOLUTIONS_AMOUNT];
    for(int j = 0; j < population.size() ; j++ ){
        for( int i = 0; i < PERIODS_AMOUNT; i++ ){
            result.tuplesNumInPeriods.push_back( population[j]->periods[i]->tuples.size() );
            for(int k = 0; k < population[j]->periods[i]->tuples.size() ; k++){
                cout << k << endl;

                result.tuplesIds.push_back(population[j]->periods[i]->tuples[k]->id);
                result.groupIds.push_back(population[j]->periods[i]->tuples[k]->groupId);
                result.lecturerIds.push_back(population[j]->periods[i]->tuples[k]->lecturerId);
                result.roomIds.push_back(population[j]->periods[i]->tuples[k]->roomId);            
            }
        }
    }
    return result;
}




SerializedPopulation deserialize(vector<Solution *> population){
    int populationSize =  

    for()
}




void broadcastPopulation(SerializedPopulation sp,  MPI_Comm world){



    int* periodsNum = new int[1];
    int* tuplesNum = new int [1];

    * periodsNum = sp.tuplesNumInPeriods.size();
    * tuplesNum = sp.roomIds.size();

        cout << *periodsNum << " " << *tuplesNum << " "<< sp.tuplesIds[10] << endl;


    MPI_Bcast(periodsNum, 1, MPI_INT, 0, world);
    MPI_Bcast(tuplesNum, 1, MPI_INT, 0, world);


    for (int i = 1; i < 8; i++ ){
        MPI_Send(periodsNum, 1, MPI_INT, i, 121, MPI_COMM_WORLD);
        MPI_Send(tuplesNum, 1, MPI_INT, i, 123, MPI_COMM_WORLD);

        MPI_Send(&sp.tuplesIds[0], *tuplesNum, MPI_INT, i, 10, MPI_COMM_WORLD);
        MPI_Send(&sp.groupIds[0], *tuplesNum, MPI_INT, i, 11, MPI_COMM_WORLD);
        MPI_Send(&sp.lecturerIds[0], *tuplesNum, MPI_INT, i, 12, MPI_COMM_WORLD);
        MPI_Send(&sp.roomIds[0], *tuplesNum, MPI_INT, i, 13, MPI_COMM_WORLD);
    }

}

SerializedPopulation recivePopulation(MPI_Comm world, MPI_Status * status){
    SerializedPopulation result;

    int* periodsNum = new int[1];
    int* tuplesNum = new int [1];

     


    MPI_Recv(periodsNum, 1, MPI_INT, 0, 121, MPI_COMM_WORLD, status);
    MPI_Recv(tuplesNum, 1, MPI_INT, 0, 123, MPI_COMM_WORLD, status);

    int * tuplesNumInPeriods = new int[*periodsNum];

    int * tuplesIds = new int[*tuplesNum];
    int * groupIds = new int[*tuplesNum];
    int * lecturerIds = new int[*tuplesNum];
    int * roomIds = new int[*tuplesNum];



    MPI_Recv(tuplesNumInPeriods, *periodsNum, MPI_INT, 0, 14, MPI_COMM_WORLD, status);

    MPI_Recv(tuplesIds, *tuplesNum, MPI_INT, 0, 10, MPI_COMM_WORLD, status);
    MPI_Recv(groupIds, *tuplesNum, MPI_INT, 0, 11, MPI_COMM_WORLD, status);
    MPI_Recv(lecturerIds, *tuplesNum, MPI_INT, 0, 12, MPI_COMM_WORLD, status);
    MPI_Recv(roomIds, *tuplesNum, MPI_INT, 0, 13, MPI_COMM_WORLD, status);


    result.tuplesNumInPeriods = vector<int>(tuplesNumInPeriods, tuplesNumInPeriods + *periodsNum);
    result.tuplesIds = vector<int>(tuplesIds, tuplesIds + *tuplesNum);
    result.groupIds = vector<int>(groupIds, groupIds + *tuplesNum);
    result.lecturerIds = vector<int>(lecturerIds, lecturerIds + *tuplesNum);
    sresult.roomIds = vector<int>(roomIds, *tuplesNum);

    return result;
}