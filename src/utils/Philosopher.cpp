#include <cstdlib>
#include <iostream>
#include "Philosopher.hpp"
#include <unistd.h>
#include <thread>
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

void Philosopher::takeRightFork()
{
    sem_wait(&this->data.dish->rightDish->semFork);
    this->data.dish->rightDish->leftFork = Fork::TAKEN;
}

void Philosopher::takeLeftFork()
{
    sem_wait(&this->data.dish->semFork);
    this->data.dish->leftFork = Fork::TAKEN;
}

void Philosopher::takeForks()
{
    this->takeRightFork();
    this->takeLeftFork();
}

void Philosopher::leaveRightFork()
{
    this->data.dish->rightDish->leftFork = Fork::AVAILABLE;
    sem_post(&this->data.dish->rightDish->semFork);
}

void Philosopher::leaveLeftFork()
{
    this->data.dish->leftFork = Fork::AVAILABLE;
    sem_post(&this->data.dish->semFork);
}

void Philosopher::leaveforks()
{
    this->leaveRightFork();
    this->leaveLeftFork();
}

void Philosopher::philosopherRoutine()
{
    while (true)
    {
        this->think();
        this->eat();
    }
}

void Philosopher::eat()
{
    const chrono::seconds numRan{1 + rand() % 11};
    {
        lock_guard lock{cout_mutex};
        cout << "Hola soy el filósofo " << this->data.id << " y tengo hambre veré si puedo comer" << endl;
    }
    this->takeForks();
    {
        lock_guard lock{cout_mutex};
        cout << "Hola soy el filósofo " << this->data.id << " y hay tenedores disponibles empezaré a comer" << endl;
    }
    this_thread::sleep_for(numRan);
    this->leaveforks();
    {
        lock_guard lock{cout_mutex};
        cout << "Hola soy el filósofo " << this->data.id << " y ya terminé de comer" << endl;
    }
}

void Philosopher::think()
{
    const chrono::seconds numRan{1 + rand() % 11};
    {
        lock_guard lock{cout_mutex};
        cout << "Hola soy el filósofo " << this->data.id << " y voy a pensar un rato" << endl;
    }
    this_thread::sleep_for(numRan);
    {
        lock_guard lock{cout_mutex};
        cout << "Hola soy el filósofo " << this->data.id << " ya me aburri de pensar" << endl;
    }
}
