#include "Philosopher.hpp"

#include <random>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

unsigned Philosopher::next_id{1};

Philosopher::Philosopher() : data{next_id++, nullptr}
{
    state = PhilosopherStates::THINKING;
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

void Philosopher::kill()
{
    killed = true;
}

static auto generate_random = mt19937{random_device{}()};

void Philosopher::eat()
{
    const chrono::seconds numRan{1 + generate_random() % 10};
    state = PhilosopherStates::WATING;
    waitCounter++;
    const auto initialTime = chrono::steady_clock::now();
    takeForks();
    const auto finalTime = chrono::steady_clock::now();
    totalWatingTime += chrono::duration_cast<chrono::seconds>(finalTime - initialTime);
    state = PhilosopherStates::EATING;
    this_thread::sleep_for(numRan);
    leaveForks();
}

void Philosopher::think()
{
    const chrono::seconds numRan{1 + generate_random() % 10};
    totalThinkingTime += numRan;
    state = PhilosopherStates::THINKING;
    thinkCounter++;
    this_thread::sleep_for(numRan);
}

unsigned Philosopher::getId() const { return data.id; }
int Philosopher::getEatCounter() { return eatCounter; }
int Philosopher::getThinkCounter() { return thinkCounter; }
int Philosopher::getWaitCounter() { return waitCounter; }
chrono::seconds Philosopher::getAverageEatingTime() { return totalEatingTime / eatCounter; }
chrono::seconds Philosopher::getAverageThinkingTime() { return totalThinkingTime / thinkCounter; }
chrono::seconds Philosopher::getAverageWatingTime() { return totalWatingTime / waitCounter; }
bool Philosopher::isEating() const { return state == PhilosopherStates::EATING; }
bool Philosopher::isDead() const { return state == PhilosopherStates::DEAD; }