#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include "Dish.hpp"
#include "Logger.hpp"
#include "Pause.hpp"
#include <atomic>
#include <chrono>
#include <iostream>
#include <shared_mutex>
#include <thread>

using namespace std;

enum class PhilosopherStates
{
    THINKING,
    EATING,
    WAITING,
    DEAD,
};

struct PhilosopherData
{
    unsigned id;
    Dish *dish;
};

class Philosopher
{
public:
    Philosopher();
    Philosopher(const Philosopher &) = delete;
    Philosopher(Philosopher &&) = delete;
    Philosopher &operator=(const Philosopher &) = delete;
    Philosopher &operator=(Philosopher &&) = delete;

    PhilosopherData getData();
    int getEatCounter() const;
    int getThinkCounter() const;
    int getWaitCounter() const;
    chrono::seconds getAverageEatingTime() const;
    chrono::seconds getAverageThinkingTime() const;
    chrono::seconds getAverageWatingTime() const;
    void beginPhilosophersLife();
    void waitTillPhilosopherDies();
    void setDish(Dish *dish);
    void kill();
    unsigned getId() const;
    string getColoredId() const;
    bool isEating() const;
    bool isDead() const;
    bool isThinking() const;
    void setMaxThink(chrono::milliseconds value) { maxThink = value; }
    void setMaxEat(chrono::milliseconds value) { maxEat = value; }

    static void registerPause(Pause *p) { pause = p; }

private:
    static unsigned next_id;
    inline static Pause *pause = nullptr;

    enum PhilosopherStates state;

    condition_variable killed_cv{};
    mutex killed_mut{};
    atomic_bool killed{false};

    int eatCounter;
    int thinkCounter;
    int waitCounter;
    chrono::milliseconds totalEatingTime;
    chrono::milliseconds totalThinkingTime;
    chrono::milliseconds totalWatingTime;
    thread lifeThread{};
    chrono::milliseconds maxThink{10s};
    chrono::milliseconds maxEat{10s};
    PhilosopherData data;
    void takeForks();
    void leaveForks();
    void philosopherRoutine();
    void eat();
    void think();

    template <typename Rep, typename Period>
    void waitTillTimeoutOrKilled(const chrono::duration<Rep, Period> &timeout)
    {
        unique_lock lck{killed_mut};
        killed_cv.wait_for(lck, timeout, [&] { return killed == true; });
    }

    void setState(PhilosopherStates newState)
    {
        pause->wait();
        state = newState;
    }
};

#endif