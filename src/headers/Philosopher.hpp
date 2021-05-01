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
    bool isThinking() const;
    void setMaxThink(chrono::seconds value) { maxThink = value; }
    void setMaxEat(chrono::seconds value) { maxEat = value; }

    static void registerPause(Pause *p) { pause = p; }

private:
    static unsigned next_id;
    inline static Pause *pause = nullptr;
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