#include <atomic>
#include <chrono>
#include <forward_list>
#include <iomanip>
#include <iostream>
#include <queue>
#include <utility>

#include "DiningTable.hpp"
#include "Logger.hpp"
#include "Philosopher.hpp"

using namespace std;

void simulate(forward_list<Philosopher> &philosophers);

enum class Event
{
    quit,
};

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(false);

    int num = argc > 1 ? atoi(argv[1]) : 3;

    auto philosophers = forward_list<Philosopher>(num);

    DiningTable table;

    table.serve(philosophers.begin(), philosophers.end());

    simulate(philosophers);

    return 0;
}

void process_input(queue<Event> &eventQueue, mutex &queueMut,
                   atomic_bool &finished)
{
    while (!finished)
    {
        char c;
        while ((c = cin.peek()) != istream::traits_type::eof() && cin &&
               c != '\n')
        {
            cin.get();
            Event e;
            if (c == 'q')
            {
                e = Event::quit;
            }
            else
            {
                continue;
            }
            lock_guard lock{queueMut};
            eventQueue.push(e);
        }

        if (!cin)
        {
            cin.clear();
        }

        this_thread::yield();
    }
}

template <typename Rep, typename Period>
double duration_to_seconds_double(const chrono::duration<Rep, Period> &d)
{
    const auto factor = double(Period::num) / double(Period::den);
    return d.count() * factor;
}

void simulate(forward_list<Philosopher> &philosophers)
{
    atomic_bool all_philosophers_died = false;
    Logger logger;
    Philosopher::registerLogger(logger);

    for (auto &phil : philosophers)
    {
        phil.beginPhilosophersLife();
    }

    constexpr auto renders_interval = 1000ms;

    auto event_queue_mutex = mutex{};
    auto event_queue = queue<Event>{};

    auto kill_all = [&] {
        logger("Matando a los filósofos. Espere un momento.");
        for_each(philosophers.begin(), philosophers.end(),
                 mem_fn(&Philosopher::kill));
    };

    auto input_thread = thread{[&] {
        process_input(event_queue, event_queue_mutex, all_philosophers_died);
    }};

    auto render_thread = thread{[&] {
        const auto start_time = chrono::steady_clock::now();

        auto last_render_time = start_time;

        while (!all_philosophers_died)
        {

            for (;;)
            {
                Event e;
                {
                    lock_guard lock{event_queue_mutex};
                    if (event_queue.empty())
                    {
                        break;
                    }
                    e = event_queue.front();
                    event_queue.pop();
                }
                switch (e)
                {
                case Event::quit:
                    kill_all();
                    break;
                }
            }

            const auto current_time = chrono::steady_clock::now();
            const auto time_since_start = current_time - start_time;
            const auto time_since_last_render = current_time - last_render_time;

            const auto should_render =
                time_since_last_render >= renders_interval;

            if (should_render)
            {
                system("clear");
                cout << duration_to_seconds_double(time_since_start) << ": ";

                for (const auto &philosopher : philosophers)
                {
                    if (philosopher.isEating())
                    {
                        cout << "\x1b[1;30;42m";
                    }
                    if (philosopher.isDead())
                    {
                        cout << "\x1b[1;31m";
                    }
                    const auto *reset = "\x1b[0m";
                    cout << " " << philosopher.getId() << " " << reset << " ";
                }
                cout << '\n' << logger;
                cout.flush();

                last_render_time = chrono::steady_clock::now();
            }
            this_thread::yield();
        }
    }};

    for (auto &phil : philosophers)
    {
        phil.waitTillPhilosopherDies();
    }

    all_philosophers_died = true;

    input_thread.join();
    render_thread.join();
}
