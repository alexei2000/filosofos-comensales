#include <cstdlib>
#include <iostream>
#include "Philosopher.hpp"
#include <unistd.h>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;

mutex Philosopher::cout_mutex{};

Philosopher::Philosopher(int id)
{
    this->data.id = id;
}

PhilosopherData Philosopher::getData()
{
    return this->data;
}

void Philosopher::setDish(Dish *dish)
{
    this->data.dish = dish;
}

void Philosopher::beginPhilosophersLife()
{
    _thread = thread{&Philosopher::philosopherRoutine, this};
}

void Philosopher::waitTillPhilosopherDies()
{
    _thread.join();
}

void Philosopher::takeForks() const
{
    data.dish->takeForks();
}

void Philosopher::leaveForks() const
{
    data.dish->leaveForks();
}

void Philosopher::philosopherRoutine()
{
    while (true)
    {
        think();
        eat();
    }
}

void Philosopher::eat() const
{
    const chrono::seconds numRan{1 + rand() % 11};
    {
        lock_guard lock{cout_mutex};
        cout << "Hola soy el filósofo "
             << this->data.id << " y tengo hambre veré si puedo comer." << endl;
    }
    takeForks();
    {
        lock_guard lock{cout_mutex};
        cout << "Hola soy el filósofo "
             << this->data.id << " y hay tenedores disponibles empezaré a comer durante "
             << numRan.count() << " segundos." << endl;
    }
    this_thread::sleep_for(numRan);
    {
        lock_guard lock{cout_mutex};
        cout << "Hola soy el filósofo " << this->data.id << " y ya terminé de comer." << endl;
    }
    leaveForks();
}

void Philosopher::think() const
{
    const chrono::seconds numRan{1 + rand() % 11};
    {
        lock_guard lock{cout_mutex};
        cout << "Hola soy el filósofo "
             << this->data.id << " y voy a pensar "
             << numRan.count() << " segundos." << endl;
    }
    this_thread::sleep_for(numRan);
    {
        lock_guard lock{cout_mutex};
        cout << "Hola soy el filósofo " << this->data.id << " ya me aburri de pensar." << endl;
    }
}
