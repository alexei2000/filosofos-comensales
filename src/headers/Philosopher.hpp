#ifndef PHILOSOPHER_H
#define PHILOSOPHER_H

#include "Dish.hpp"
#include "Logger.hpp"

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

using namespace std;

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
    void beginPhilosophersLife();
    void waitTillPhilosopherDies();
    void setDish(Dish *dish);
    void kill();
    unsigned getId() const;
    string getColoredId() const;
    bool isEating() const;
    bool isDead() const;

    static void registerLogger(Logger &l);
    static void log(const string &);

private:
    static unsigned next_id;
    static Logger *logger_ptr;
    atomic_bool killed{false};
    atomic_bool died{false};
    atomic_bool eating{false};
    thread lifeThread{};
    PhilosopherData data;
    void takeForks() const;
    void leaveForks() const;
    void philosopherRoutine();
    void eat();
    void think() const;
};

#endif