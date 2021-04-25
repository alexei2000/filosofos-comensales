#if !defined(DISH_HPP_)
#define DISH_HPP_

#include <semaphore.h>

enum class Fork
{
  TAKEN,
  AVAILABLE,
};

class Dish
{
  friend class DiningTable;

  Fork leftFork = Fork::AVAILABLE;
  Dish *rightDish = nullptr;
  sem_t semFork;

public:
  Dish() : semFork{}
  {
    sem_init(&semFork, 0, 1);
  }

  void takeForks()
  {
    takeLeftFork();
    takeRightFork();
  }

  void leaveForks()
  {
    leaveLeftFork();
    leaveRightFork();
  }

private:
  void takeRightFork() const
  {
    sem_wait(&rightDish->semFork);
    rightDish->leftFork = Fork::TAKEN;
  }

  void takeLeftFork()
  {
    sem_wait(&semFork);
    leftFork = Fork::TAKEN;
  }

  void leaveRightFork()
  {
    rightDish->leftFork = Fork::AVAILABLE;
    sem_post(&rightDish->semFork);
  }

  void leaveLeftFork()
  {
    leftFork = Fork::AVAILABLE;
    sem_post(&semFork);
  }
};

#endif // DISH_HPP_
