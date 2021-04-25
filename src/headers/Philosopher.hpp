#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include "Dish.hpp"

struct PhilosopherData {
  int id;
  Dish *dish;
};

class Philosopher {
 public:
  Philosopher(int id);
  PhilosopherData getData();
  void philosophersLife();
  void setDish(Dish *dish);

 private:
  PhilosopherData data;
  void takeForks();
  void takeRightFork();
  void takeLeftFork();
  void leaveForks();
  void leaveRightFork();
  void leaveLeftFork();
  void leaveforks();
  void philosopherRoutine();
  void eat();
  void think();
};

#endif