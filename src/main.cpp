#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <memory>
#include <utility>
#include <vector>

#include "DiningTable.hpp"
#include "Philosopher.hpp"

using namespace std;

pair<vector<Philosopher>, unique_ptr<DiningTable>> createPhilosophers(int num);
void simulate(vector<Philosopher> &philosophers);

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int num = atoi(argv[1]);
    auto [philosophers, table] = createPhilosophers(num);
    simulate(philosophers);
    return 0;
}

pair<vector<Philosopher>, unique_ptr<DiningTable>> createPhilosophers(int num)
{
    vector<Philosopher> philosophers;
    for (int i = 0; i < num; i++)
    {
        philosophers.emplace_back(i);
    }
    return {philosophers, make_unique<DiningTable>(philosophers)};
}

void simulate(vector<Philosopher> &philosophers)
{
    for (int i = 0; i < philosophers.size(); i++)
    {
        philosophers[i].philosophersLife();
    }
}
