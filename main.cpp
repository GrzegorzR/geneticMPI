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



using namespace std;

const int INITIAL_SOLUTIONS_AMOUNT = 10;
//SOLUTION PARAMETERS

const int TUPLES_AMOUNT = 250;
const int PERIODS_AMOUNT = 50;
const int LECTURERS_AMOUNT = 10;
const int ROOMS_AMOUNT = 8;
const int GROUPS_AMOUNT = 10;

class Tuple
{
  public:
    int id, groupId, lecturerId, roomId;
    Tuple(int id, int lecturerId, int groupId, int roomId)
    {
        this->id = id;
        this->groupId = groupId;
        this->lecturerId = lecturerId;
    }
};

vector<Tuple *> tuples_org;

class Period
{
  public:
    vector<Tuple *> tuples;
    Period() {}
    void addTuple(Tuple *tuple)
    {
        this->tuples.push_back(tuple);
    }
};

class Solution
{
  public:
    vector<Period *> periods;
    Solution(vector<Period *> periods)
    {
        this->periods = periods;
    }

    int cost()
    {
        int cost = 0;
        for (int i = 0; i < periods.size(); i++)
        {
            Period *period = periods[i];
            set<int> usedRoomIds;
            set<int> usedLecturersIds;
            set<int> usedGroupIds;
            for (int j = 0; j < period->tuples.size(); j++)
            {
                Tuple *tuple = period->tuples[j];
                if (usedRoomIds.find(tuple->roomId) != usedLecturersIds.end())
                {
                    cost++;
                }
                if (usedLecturersIds.find(tuple->roomId) != usedLecturersIds.end())
                {
                    cost++;
                }
                if (usedGroupIds.find(tuple->roomId) != usedGroupIds.end())
                {
                    cost++;
                }
                usedRoomIds.insert(tuple->roomId);
                usedLecturersIds.insert(tuple->lecturerId);
                usedGroupIds.insert(tuple->groupId);
            }
        }
        return cost;
    }

    Tuple *getTupleById(int id)
    {
        for (int i = 0; i < periods.size(); i++)
        {
            Period *period = periods[i];
            for (int j = 0; j < period->tuples.size(); j++)
            {
                Tuple *tuple = period->tuples[j];
                if (tuple->id == id)
                {
                    return tuple;
                }
            }
        }
    }
    void recombine()
    {
        //prepare
        vector<int> duplicatedIds;
        vector<int> missingIds;
        std::set<int> ids;
        for (int i = 0; i < periods.size(); i++)
        {
            Period *period = periods[i];
            for (int j = 0; j < period->tuples.size(); j++)
            {
                int tupleId = period->tuples[j]->id;
                if (ids.find(tupleId) == ids.end())
                {
                    ids.insert(tupleId);
                }
                else
                {
                    duplicatedIds.push_back(tupleId);
                }
            }
        }
        for (int i = 0; i < tuples_org.size(); i++)
        {
            int tupleId = tuples_org[i]->id;
            if (ids.find(tupleId) == ids.end())
            {
                missingIds.push_back(tupleId);
            }
        }

        for (int i = 0; i < duplicatedIds.size(); i++)
        {
            int duplicatedTupleId = duplicatedIds[i];
            Tuple *dup_Tuple = getTupleById(duplicatedTupleId);
            // cout << "tuple by id end" << endl;
            Tuple *orginalTuple = tuples_org[missingIds[0]];
            // cout << "orginal tuple id end" << endl;
            missingIds.erase(missingIds.begin());

            //reasign values
            dup_Tuple->id = orginalTuple->id;
            dup_Tuple->groupId = orginalTuple->groupId;
            dup_Tuple->lecturerId = orginalTuple->lecturerId;
            dup_Tuple->roomId = orginalTuple->roomId;
        }
    }

    void mutate()
    {
        // cout<<"Mutation not implements yet"<<endl;
    }
    void printToConsole()
    {
        for (int i = 0; i < this->periods.size(); i++)
        {
            std::cout << "Period " << i << endl;
            Period *period = this->periods.at(i);
            for (int j = 0; j < period->tuples.size(); j++)
            {
                Tuple *tuple = period->tuples.at(j);
                std::cout << "id: " << tuple->id << ", groupId: " << tuple->groupId << ", lecturerId: " << tuple->lecturerId << ", roomId: " << tuple->roomId << endl;
                // printf ("%s \n", "A string");
            }
        }
    }
};

int getRandRangeInt(int beg, int end)
{
    return rand() % end + beg;
}

vector<Tuple *> createSolutionTuples()
{
    std::vector<Tuple *> tuples;
    int id = 0;
    for (int classesId = 0; classesId < TUPLES_AMOUNT; classesId++)
    {
        int lecturerId = getRandRangeInt(0, LECTURERS_AMOUNT);
        int groupId = getRandRangeInt(0, GROUPS_AMOUNT);
        int roomId = getRandRangeInt(0, ROOMS_AMOUNT);

        Tuple *tuple = new Tuple(id++, lecturerId, groupId, roomId);
        tuples.push_back(tuple);
    }
    return tuples;
}

Solution *generateRandomSolution(vector<Tuple *> tuples, int periodsAmount)
{
    //initialize
    std::vector<Period *> periods(periodsAmount);
    for (int i = 0; i < periodsAmount; i++)
    {
        periods[i] = new Period();
    }
    //
    for (int i = 0; i < tuples.size(); i++)
    {
        int periodId = getRandRangeInt(0, periodsAmount);
        Tuple *tuple = tuples[i];
        periods[periodId]->addTuple(new Tuple(tuple->id, tuple->groupId, tuple->lecturerId, tuple->roomId));
    }
    return new Solution(periods);
}

vector<Solution *> createPopulation(int solutionsAmount)
{
    vector<Solution *> solutions;
    for (int i = 0; i < solutionsAmount; i++)
    {
        solutions.push_back(generateRandomSolution(tuples_org, PERIODS_AMOUNT));
    }
    return solutions;
}

Period *periodsCrossover(Period *p1, Period *p2)
{
    int p1_tuplesAmount = p1->tuples.size();
    int p2_tuplesAmount = p2->tuples.size();
    int smallerAmount = p1_tuplesAmount > p2_tuplesAmount ? p2_tuplesAmount : p1_tuplesAmount;

    Period *p_result = new Period();

    int amountOfNotChangingTuples = getRandRangeInt(0, smallerAmount + 1);
    //reasign not changed values
    for (int i = 0; i < amountOfNotChangingTuples; i++)
    {
        p_result->addTuple(p1->tuples[i]);
    }
    // assign new tuples
    for (int i = amountOfNotChangingTuples; i < p2->tuples.size(); i++)
    {
        p_result->addTuple(p2->tuples[i]);
    }
    return p_result;
}

Solution *crossSolutions(Solution *s1, Solution *s2)
{
    vector<Period *> periods_result;
    for (int i = 0; i < s1->periods.size(); i++)
    {
        Period *crossOveredPeriod = periodsCrossover(s1->periods[i], s2->periods[i]);
        periods_result.push_back(crossOveredPeriod);
    }
    return new Solution(periods_result);
}

vector<Solution *> naturalSelection(vector<Solution *> population)
{
    vector<int> costs(population.size());
    for (int i = 0; i < population.size(); i++)
    {
        costs.push_back(population[i]->cost());
    }
    std::sort(costs.rbegin(), costs.rend());
    int threshold = population.size() * 0.1 > 10 ? costs.at(int(population.size() * 0.1)) : costs.at(0);

    vector<Solution *> populationSelected;
    for (int i = 0; i < population.size(); i++)
    {
        if (population[i]->cost() < threshold)
        {
            populationSelected.push_back(population[i]);
        }
    }
    return populationSelected;
}

bool isSolutionSatisfying(vector<Solution *> population)
{
    return false;
}

float POPULATION_INCREASE_FAKTOR = 1;

int main(int argc,char** argv)
{
    //test

 int size, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (size < 2) {
        if (rank == 0)
            std::cerr << "Require at least 2 tasks" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

   const int lentag=0;
   const int datatag=1;
   if (rank == 0) {
        int nums[] = {1,4,9,16};
        std::set<int> send_set(nums, nums+4);
        

        std::cout << "Rank " << rank << " sending set: ";
        for (std::set<int>::iterator i=send_set.begin(); i!=send_set.end(); i++)
            std::cout << *i << " ";
        std::cout << std::endl;

        // We're going to serialize into a std::string of bytes, and then send this
        std::string serial_str;
        boost::iostreams::back_insert_device<std::string> inserter(serial_str);
        boost::iostreams::stream<boost::iostreams::back_insert_device<std::string> > s(inserter);
        boost::archive::binary_oarchive send_ar(s);

        send_ar << send_set;
        s.flush();
        int len = serial_str.size();

        // Send length, then data
        MPI_Bcast( &len, 1, MPI_INT, 1, lentag, MPI_COMM_WORLD );
        MPI_Bcast( (void *)serial_str.data(), len, MPI_BYTE, 1, datatag, MPI_COMM_WORLD );
    } else  {
        int len;
        MPI_Recv( &len, 1, MPI_INT, 0, lentag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        char data[len+1];
        MPI_Recv( data, len, MPI_BYTE, 0, datatag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        data[len] = '\0';

        boost::iostreams::basic_array_source<char> device(data, len);
        boost::iostreams::stream<boost::iostreams::basic_array_source<char> > s(device);
        boost::archive::binary_iarchive recv_ar(s);

        std::set<int> recv_set;
        recv_ar >> recv_set;

        std::cout << "Rank " << rank << " got set: ";
        for (std::set<int>::iterator i=recv_set.begin(); i!=recv_set.end(); i++)
            std::cout << *i << " ";
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;

    /*
    tuples_org = createSolutionTuples();
    vector<Solution *> population = createPopulation(10);
    // epoch iterator
    while (population.size() > 2 && !isSolutionSatisfying(population))
    {
        cout << "New epoch, population size: " << population.size() << endl;
        vector<Solution *> new_population;
        for (int i = 1; i < 10; i++)
        {
            int index1 = getRandRangeInt(0, population.size());
            int index2 = getRandRangeInt(0, population.size());
            cout << "Mutated solutions: " << index1 << ", " << index2 << endl;
            Solution *new_solution = crossSolutions(population[index1], population[index2]);
            new_solution->recombine();
            new_solution->mutate();
            new_population.push_back(new_solution);
        }
        population = naturalSelection(new_population);
        cout << "Solution costs: ";
        for (int i = 0; i < population.size(); i++)
        {
            cout << population[0]->cost() << ", ";
        }
        cout << endl;
    }
    cout << "Solution cost" << population[0]->cost() << endl;
    cout << "End of execution, press any key to end" << endl;
    cin.get();
    return 0;
    */
}