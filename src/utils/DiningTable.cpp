#include "DiningTable.hpp"
#include <new>
#include <semaphore.h>

DiningTable::DiningTable(std::vector<Philosopher> &philosophers) : firstDish{nullptr}, lastDish{nullptr}
{
    for (auto &phil : philosophers)
    {
        phil.setDish(this->insertNode());
    }
    closeList();
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
    Dish *newDish = new Dish();

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
