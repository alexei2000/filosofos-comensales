#include "Philosopher.hpp"

#include <functional>
#include <random>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

template <typename Func, typename... Args>
auto measureTime(Func &&f, Args &&...args) -> chrono::milliseconds
{
    const auto initialTime = chrono::steady_clock::now();

    invoke(forward<Func>(f), forward<Args>(args)...);

    const auto finalTime = chrono::steady_clock::now();

    auto delta = finalTime - initialTime;

    return chrono::duration_cast<chrono::milliseconds>(delta);
}

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

void Philosopher::takeForks()
{
    totalWatingTime += measureTime(&Dish::takeForks, data.dish);
    waitCounter++;
}

void Philosopher::leaveForks() { data.dish->leaveForks(); }

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
    setState(PhilosopherStates::DEAD);
}

void Philosopher::kill()
{
    lock_guard lck{killed_mut};
    killed = true;
    killed_cv.notify_one();
}

static auto generate_random = mt19937{random_device{}()};

void Philosopher::eat()
{
    const chrono::milliseconds numRan{1 + generate_random() % maxEat.count()};

    takeForks();

    setState(PhilosopherStates::EATING);

    totalEatingTime += measureTime([&] { waitTillTimeoutOrKilled(numRan); });
    eatCounter++;

    leaveForks();

    setState(PhilosopherStates::WAITING);
}

void Philosopher::think()
{
    const chrono::milliseconds numRan{1 + generate_random() % maxThink.count()};

    setState(PhilosopherStates::THINKING);

    totalThinkingTime += measureTime([&] { waitTillTimeoutOrKilled(numRan); });
    thinkCounter++;

    setState(PhilosopherStates::WAITING);
}

unsigned Philosopher::getId() const { return data.id; }

int Philosopher::getEatCounter() const { return eatCounter; }
int Philosopher::getThinkCounter() const { return thinkCounter; }
int Philosopher::getWaitCounter() const { return waitCounter; }

chrono::seconds Philosopher::getAverageEatingTime() const
{
    return chrono::duration_cast<chrono::seconds>(
        eatCounter ? totalEatingTime / eatCounter : 0s);
}

chrono::seconds Philosopher::getAverageThinkingTime() const
{
    return chrono::duration_cast<chrono::seconds>(
        thinkCounter ? totalThinkingTime / thinkCounter : 0s);
}

chrono::seconds Philosopher::getAverageWatingTime() const
{
    return chrono::duration_cast<chrono::seconds>(
        waitCounter ? totalWatingTime / waitCounter : 0s);
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
