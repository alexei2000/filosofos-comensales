#include "DiningTable.hpp"

#include <semaphore.h>

DiningTable::DiningTable() : firstDish{nullptr}, lastDish{nullptr} {}

DiningTable::~DiningTable()
{
    if (firstDish == nullptr)
    {
        return;
    }

    lastDish->rightDish = nullptr;
    while (firstDish != nullptr)
    {
        Dish *aux = firstDish;
        firstDish = firstDish->rightDish;
        delete aux;
    }
    firstDish = lastDish = nullptr;
}

Dish *DiningTable::insertNode()
{
    auto *newDish = new Dish;
    if (firstDish == nullptr)
    {
        lastDish = firstDish = newDish;
    }
    else
    {
        lastDish->rightDish = newDish;
        lastDish = newDish;
    }

    newDish->rightDish = firstDish;
    return newDish;
}
