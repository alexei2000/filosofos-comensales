#include "Philosopher.hpp"

#include <random>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

unsigned Philosopher::next_id{1};

Philosopher::Philosopher() : data{next_id++, nullptr}
{
    state = PhilosopherStates::WAITING;
    eatCounter = 0;
    thinkCounter = 0;
    waitCounter = 0;
    totalThinkingTime = 0s;
    totalEatingTime = 0s;
    totalWatingTime = 0s;
}

PhilosopherData Philosopher::getData() { return this->data; }

void Philosopher::setDish(Dish *dish) { this->data.dish = dish; }

void Philosopher::beginPhilosophersLife()
{
    lifeThread = thread{&Philosopher::philosopherRoutine, this};
}

void Philosopher::waitTillPhilosopherDies() { lifeThread.join(); }

void Philosopher::takeForks() const { data.dish->takeForks(); }

void Philosopher::leaveForks() const { data.dish->leaveForks(); }

void Philosopher::philosopherRoutine()
{
    while (!killed)
    {
        think();

        if (killed)
        {
            break;
        }

        eat();
    }
    state = PhilosopherStates::DEAD;
}

void Philosopher::kill() { killed = true; }

static auto generate_random = mt19937{random_device{}()};

void Philosopher::eat()
{
    const chrono::seconds numRan{1 + generate_random() % maxEat.count()};
    const auto initialTime = chrono::steady_clock::now();
    takeForks();
    const auto finalTime = chrono::steady_clock::now();
    totalWatingTime +=
        chrono::duration_cast<chrono::seconds>(finalTime - initialTime);
    pause->wait();
    waitCounter++;
    state = PhilosopherStates::EATING;
    this_thread::sleep_for(numRan);
    leaveForks();
    pause->wait();
    state = PhilosopherStates::WAITING;
}

void Philosopher::think()
{
    const chrono::seconds numRan{1 + generate_random() % maxThink.count()};
    pause->wait();
    state = PhilosopherStates::THINKING;
    this_thread::sleep_for(numRan);
    totalThinkingTime += numRan;
    thinkCounter++;
}

unsigned Philosopher::getId() const { return data.id; }

int Philosopher::getEatCounter() { return eatCounter; }
int Philosopher::getThinkCounter() { return thinkCounter; }
int Philosopher::getWaitCounter() { return waitCounter; }
chrono::seconds Philosopher::getAverageEatingTime()
{
    return totalEatingTime / eatCounter;
}
chrono::seconds Philosopher::getAverageThinkingTime()
{
    return totalThinkingTime / thinkCounter;
}
chrono::seconds Philosopher::getAverageWatingTime()
{
    return totalWatingTime / waitCounter;
}
bool Philosopher::isEating() const
{
    return state == PhilosopherStates::EATING;
}
bool Philosopher::isDead() const { return state == PhilosopherStates::DEAD; }

bool Philosopher::isThinking() const
{
    return state == PhilosopherStates::THINKING;
}

string Philosopher::getColoredId() const
{
    stringstream ss;

    if (isEating())
    {
        ss << "\x1b[7;32m";
    }
    else if (isDead())
    {
        ss << "\x1b[1;31m";
    }
    else if (isThinking())
    {
        ss << "\x1b[7m";
    }

    ss << ' ' << getId() << ' ';
    ss << "\x1b[0m";

    return ss.str();
}
