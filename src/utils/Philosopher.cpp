#include "Philosopher.hpp"
#include <chrono>
#include <random>
#include <sstream>
#include <thread>
#include <unistd.h>
#include <vector>

using namespace std;

unsigned Philosopher::next_id{1};
Logger *Philosopher::logger_ptr{nullptr};

Philosopher::Philosopher() : data{next_id++, nullptr} {}

void Philosopher::registerLogger(Logger &l) { logger_ptr = &l; }

void Philosopher::log(const string &text)
{
    if (logger_ptr != nullptr)
    {
        (*logger_ptr)(text);
    }
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
    died = true;
    log("Murió el filósofo: " + to_string(getId()));
}

void Philosopher::kill()
{
    killed = true;
    log("Han asesinado al filósofo: " + to_string(getId()));
}

static auto generate_random = mt19937{random_device{}()};

void Philosopher::eat()
{
    const chrono::seconds numRan{1 + generate_random() % 10};

    log("El filósofo " + to_string(getId()) + " quiere comer por " +
        to_string(numRan.count()) + " segundos.");

    takeForks();
    eating = true;
    log("El filósofo " + to_string(getId()) + " empezó a comer.");
    this_thread::sleep_for(numRan);
    leaveForks();
    eating = false;
    log("El filósofo " + to_string(getId()) + " terminó de comer.");
}

void Philosopher::think() const
{
    const chrono::seconds numRan{1 + generate_random() % 11};
    this_thread::sleep_for(numRan);
}

unsigned Philosopher::getId() const { return data.id; }

string Philosopher::getColoredId() const
{
    stringstream ss;

    if (isEating())
    {
        ss << "\x1b[1;30;42m";
    }
    if (isDead())
    {
        ss << "\x1b[1;31m";
    }

    ss << ' ' << getId() << ' ';
    ss << "\x1b[0m";

    return ss.str();
}

bool Philosopher::isEating() const { return eating; }

bool Philosopher::isDead() const { return died; }