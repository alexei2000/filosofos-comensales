#include <atomic>
#include <chrono>
#include <forward_list>
#include <iomanip>
#include <iostream>
#include <queue>
#include <shared_mutex>
#include <utility>

#include "DiningTable.hpp"
#include "Logger.hpp"
#include "Pause.hpp"
#include "Philosopher.hpp"
#include "parseArgs.hpp"

using namespace std;

void simulate(forward_list<Philosopher> &philosophers,
              const Settings &settings);

void showStatistics(forward_list<Philosopher> &philosophers);

enum class Event
{
    none,
    quit,
    pause,
    play
};

int main(int argc, char *argv[])
{
    ios_base::sync_with_stdio(false);

    const auto settings =
        parseArgs(vector<const char *>(argv + 1, argv + argc));

    auto philosophers =
        forward_list<Philosopher>(settings.getCantidadFilosofos());

    for (auto &phil : philosophers)
    {
        phil.setMaxThink(duration_cast<milliseconds>(
            settings.getMaxThinkingTime() * settings.getSpeedFactor()));
        phil.setMaxEat(duration_cast<milliseconds>(settings.getMaxEatingTime() *
                                                   settings.getSpeedFactor()));
    }

    DiningTable table;

    table.serve(philosophers.begin(), philosophers.end());

    simulate(philosophers, settings);
    showStatistics(philosophers);
}

void showStatistics(forward_list<Philosopher> &philosophers)
{
    for (auto &phil : philosophers)
    {
        cout << "filósofo: " << phil.getId()
             << "\n\tCantidad de veces que comió: " << phil.getEatCounter()
             << "\n\tCantidad de veces que pensó: " << phil.getThinkCounter()
             << "\n\tCantidad de veces que esperó: " << phil.getWaitCounter()
             << "\n\tTiempo de comer promedio: "
             << phil.getAverageEatingTime().count() << "s"
             << "\n\tTiempo de pensar promedio: "
             << phil.getAverageThinkingTime().count() << "s"
             << "\n\tTiempo de esperar promedio: "
             << phil.getAverageWatingTime().count() << "s" << endl
             << endl;
    }
}

template <typename EventSender>
void process_input(EventSender send_event, atomic_bool &finished)
{
    while (!finished)
    {

        if (cin && cin.rdbuf()->in_avail() > 0)
        {
            char c = cin.get();
            Event e;
            switch (c)
            {
            case 'q':
                e = Event::quit;
                break;
            case 'p':
                e = Event::pause;
                break;
            case 'o':
                e = Event::play;
                break;
            case '\n':
            default:
                continue;
            }

            send_event(e);
        }

        if (!cin)
        {
            cin.clear();
        }

        this_thread::yield();
    }
}

void simulate(forward_list<Philosopher> &philosophers, const Settings &settings)
{
    atomic_bool all_philosophers_died = false;

    const auto renders_interval = 1000ms * settings.getSpeedFactor();

    Pause pause_handle;

    auto _event_queue_mutex = mutex{};
    auto _event_queue = queue<Event>{};

    auto send_event = [&](Event e) {
        lock_guard lock{_event_queue_mutex};
        _event_queue.push(e);
    };

    auto pop_event = [&]() -> Event {
        lock_guard lock{_event_queue_mutex};
        if (_event_queue.empty())
        {
            return Event::none;
        }
        auto e = _event_queue.front();
        _event_queue.pop();
        return e;
    };

    auto kill_all = [&] {
        for (auto &phil : philosophers)
        {
            phil.kill();
        }
    };

    Philosopher::registerPause(&pause_handle);

    auto pause = [&] {
        cout << "\nPausando simulación.\n";
        pause_handle.pause();
        showStatistics(philosophers);
    };

    auto play = [&] {
        cout << "\nContinuando simulación.\n";
        pause_handle.play();
    };

    for (auto &phil : philosophers)
    {
        phil.beginPhilosophersLife();
    }

    auto input_thread =
        thread{[&] { process_input(send_event, all_philosophers_died); }};

    mutex ready_mutex;
    condition_variable ready_cv;
    bool ready = false;

    auto notify_ready = [&] {
        lock_guard lck{ready_mutex};
        ready = true;
        ready_cv.notify_one();
    };

    auto wait_till_ready = [&] {
        unique_lock lk{ready_mutex};
        ready_cv.wait(lk, [&] { return ready; });
    };

    auto render_thread = thread{[&] {
        const auto start_time = chrono::steady_clock::now();

        auto last_render_time = start_time;
        chrono::steady_clock::duration pause_time = 0s;

        do
        {
            for (Event e = pop_event(); e != Event::none; e = pop_event())
            {
                switch (e)
                {
                case Event::quit:
                    notify_ready();
                    break;
                case Event::pause:
                    pause();
                    break;
                case Event::play:
                    play();
                    break;
                }
            }

            const auto current_time = chrono::steady_clock::now();
            const auto time_since_start = current_time - start_time;
            const auto time_since_last_render = current_time - last_render_time;

            const auto should_render =
                time_since_last_render >= renders_interval;

            if (time_since_start - pause_time >
                    (settings.getDuracionSimulacion() + 1s) *
                        settings.getSpeedFactor() &&
                !ready)
            {
                send_event(Event::quit);
            }

            if (should_render)
            {
                if (pause_handle.isPaused())
                {
                    pause_time += time_since_last_render;
                }
                else
                {
                    // cout << "\x1b[1000D";
                    const auto virtual_time_elapsed =
                        chrono::duration_cast<chrono::seconds>(
                            (time_since_start - pause_time) /
                            settings.getSpeedFactor())
                            .count();
                    const auto virtual_total_time =
                        settings.getDuracionSimulacion().count();
                    cout << virtual_time_elapsed << " / " << virtual_total_time
                         << " secs: ";

                    for (const auto &philosopher : philosophers)
                    {
                        cout << philosopher.getColoredId() << " ";
                    }
                    cout << endl;
                }
                last_render_time = chrono::steady_clock::now();
            }
            this_thread::yield();
        } while (!all_philosophers_died);
    }};

    wait_till_ready();
    kill_all();

    for (auto &phil : philosophers)
    {
        phil.waitTillPhilosopherDies();
    }

    all_philosophers_died = true;

    input_thread.join();
    render_thread.join();
}
