#if !defined(DISH_HPP_)
#define DISH_HPP_

#include <semaphore.h>

enum class Fork {
  TAKEN,
  AVAILABLE,
};

struct Dish {
  Fork leftFork;
  sem_t semFork;
  Dish *rightDish = nullptr;
};

#endif  // DISH_HPP_
