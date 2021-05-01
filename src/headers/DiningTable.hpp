#ifndef DINING_TABLE_H
#define DINING_TABLE_H

#include <algorithm>

#include "Dish.hpp"
#include "Philosopher.hpp"

class DiningTable
{
public:
    DiningTable();
    DiningTable(const DiningTable &) = delete;
    DiningTable(DiningTable &&) = delete;
    DiningTable &operator=(const DiningTable &) = delete;
    DiningTable &operator=(DiningTable &&) = delete;
    ~DiningTable();
    Dish *insertNode();

    template <typename FwdIter> void serve(FwdIter first, FwdIter last)
    {
        for (; first != last; first++)
        {
            Philosopher &phil = *first;
            phil.setDish(insertNode());
        }
    }

private:
    Dish *firstDish;
    Dish *lastDish;
};

#endif