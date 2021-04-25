#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include "Dish.hpp"
#include <thread>
#include <mutex>

using namespace std;

struct PhilosopherData
{
  int id;
  Dish *dish;
};

class Philosopher
{
public:
  Philosopher(int id);
  PhilosopherData getData();
  void beginPhilosophersLife();
  void waitTillPhilosopherDies();
  void setDish(Dish *dish);

private:
  static mutex cout_mutex;
  PhilosopherData data;
  thread _thread;
  void takeForks() const;
  void leaveForks() const;
  void philosopherRoutine();
  void eat() const;
  void think() const;
};

#endif