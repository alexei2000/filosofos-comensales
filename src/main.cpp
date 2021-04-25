#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "DiningTable.hpp"
#include "Philosopher.hpp"
#include <vector>

using namespace std;

vector<Philosopher> createPhilosophers(int num);
void createDiningTable(vector<Philosopher> &Philosophers);
void simulate(vector<Philosopher> &philosophers);

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int num = atoi(argv[1]);
    vector<Philosopher> philosophers = createPhilosophers(num);
    simulate(philosophers);
    return 0;
}

vector<Philosopher> createPhilosophers(int num)
{
    vector<Philosopher> philosophers;
    for (int i = 0; i < num; i++)
    {
        Philosopher p(i);
        philosophers.push_back(p);
    }
    createDiningTable(philosophers);
}

void createDiningTable(vector<Philosopher> &Philosophers)
{
    DiningTable diningTable(Philosophers);
}

void simulate(vector<Philosopher> &philosophers)
{
    for (int i = 0; i < philosophers.size(); i++)
    {
        philosophers[i].philosophersLife();
    }
}
