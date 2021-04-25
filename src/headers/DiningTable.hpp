#ifndef DINING_TABLE_H
#define DINING_TABLE_H

#include "Philosopher.hpp"
#include <vector>
#include <semaphore.h>

enum class Fork
{
    TAKEN,
    AVAILABLE,
};

struct Dish
{
    enum Fork leftFork;
    sem_t semFork;
    Dish *rightDish = NULL;
};

class DiningTable
{
public:
    DiningTable(std::vector<Philosopher> &philosophers);
    ~DiningTable();

private:
    Dish *firstDish;
    Dish *lastDish;
    Dish *insertNode();
    void closeList();
};

#endif