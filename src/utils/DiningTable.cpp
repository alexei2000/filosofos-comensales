#include "DiningTable.hpp"
#include <new>
#include <semaphore.h>

DiningTable::DiningTable(std::vector<Philosopher> &philosophers) : firstDish{nullptr}, lastDish{nullptr}
{
    for (int i = 0; i < philosophers.size(); i++)
    {
        philosophers[i].setDish(this->insertNode());
    }
    this->closeList();
}

DiningTable::~DiningTable()
{
    lastDish->rightDish = nullptr;
    for (Dish *i = firstDish; i != nullptr; i = i->rightDish)
    {
        Dish *aux = i;
        delete aux;
    }
    firstDish = lastDish = nullptr;
}

Dish *DiningTable::insertNode()
{
    Dish *newDish = new Dish;
    newDish->rightDish = NULL;
    newDish->leftFork = Fork::AVAILABLE;
    sem_init(&newDish->semFork, 0, 1);
    if (this->firstDish == NULL)
    {
        this->firstDish = newDish;
        this->lastDish = newDish;
    }
    else
    {
        this->lastDish->rightDish = newDish;
        this->lastDish = newDish;
    }
    return newDish;
}

void DiningTable::closeList()
{
    this->lastDish->rightDish = this->firstDish;
}
