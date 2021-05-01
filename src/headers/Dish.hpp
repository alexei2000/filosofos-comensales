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
    friend void printelbeta(const Dish *);

    inline static unsigned next_id{1};
    unsigned id = next_id++;

    Fork leftFork = Fork::AVAILABLE;
    Dish *rightDish = nullptr;
    sem_t semFork;

public:
    Dish() : semFork{} { sem_init(&semFork, 0, 1); }

    Dish(const Dish &) = delete;
    Dish(Dish &&) = delete;
    Dish &operator=(const Dish &) = delete;
    Dish &operator=(Dish &&) = delete;

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
