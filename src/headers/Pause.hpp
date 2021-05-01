#if !defined(PAUSE_HPP_)
#define PAUSE_HPP_

#include <iostream>
#include <mutex>
#include <shared_mutex>

using namespace std;

class Pause
{
private:
    condition_variable_any cv{};
    shared_mutex cv_m{};
    bool paused = false;

public:
    Pause() {}

    void pause()
    {
        {
            lock_guard lck{cv_m};
            paused = true;
        }
        cv.notify_all();
    }

    void play()
    {
        {
            lock_guard lck{cv_m};
            paused = false;
        }
        cv.notify_all();
    }

    void wait()
    {
        shared_lock lck{cv_m};
        cv.wait(lck, [&] { return paused == false; });
    }

    bool isPaused() { return paused; }
};

#endif // PAUSE_HPP_
