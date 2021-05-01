#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include "Dish.hpp"
#include "Logger.hpp"
#include <chrono>
#include <atomic>
#include <iostream>
#include <thread>

using namespace std;

enum class PhilosopherStates
{
    THINKING,
    EATING,
    WATING,
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
    int getEatCounter();
    int getThinkCounter();
    int getWaitCounter();
    chrono::seconds getAverageEatingTime();
    chrono::seconds getAverageThinkingTime();
    chrono::seconds getAverageWatingTime();
    void beginPhilosophersLife();
    void waitTillPhilosopherDies();
    void setDish(Dish *dish);
    void kill();
    unsigned getId() const;
    string getColoredId() const;
    bool isEating() const;
    bool isDead() const;
    void setMaxThink(chrono::seconds value) { maxThink = value; }
    void setMaxEat(chrono::seconds value) { maxEat = value; }

private:
    static unsigned next_id;
    enum PhilosopherStates state;
    bool killed{false};
    int eatCounter;
    int thinkCounter;
    int waitCounter;
    chrono::seconds totalEatingTime;
    chrono::seconds totalThinkingTime;
    chrono::seconds totalWatingTime;
    thread lifeThread{};
    chrono::seconds maxThink{10s};
    chrono::seconds maxEat{10s};
    PhilosopherData data;
    void takeForks() const;
    void leaveForks() const;
    void philosopherRoutine();
    void eat();
    void think();
};

#endif