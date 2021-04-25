#ifndef DINING_TABLE_H
#define DINING_TABLE_H

#include <vector>

#include "Dish.hpp"
#include "Philosopher.hpp"

class DiningTable {
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