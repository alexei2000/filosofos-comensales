#include <cstdlib>
#include <ctime>
#include <utility>
#include <vector>

#include "DiningTable.hpp"
#include "Philosopher.hpp"

using namespace std;

vector<Philosopher> createPhilosophers(int num);
void simulate(vector<Philosopher> &philosophers);

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int num = argc > 1 ? atoi(argv[1]) : 3;
    auto philosophers = createPhilosophers(num);
    DiningTable table{philosophers};
    simulate(philosophers);
    return 0;
}

vector<Philosopher> createPhilosophers(int num)
{
    vector<Philosopher> philosophers;

    for (int i = 0; i < num; i++)
    {
        philosophers.emplace_back(i);
    }

    return philosophers;
}

void simulate(vector<Philosopher> &philosophers)
{
    for (auto &phil : philosophers)
    {
        phil.philosophersLife();
    }
}
