#if !defined(LOGGER_HPP_)
#define LOGGER_HPP_

#include <deque>
#include <mutex>
#include <ostream>
#include <string>

class Logger
{
private:
    std::deque<std::string> logs{};
    mutable std::mutex mut{};
    unsigned max_logs = 10;

public:
    Logger() {}

    void operator()(const std::string &text)
    {
        static size_t i = 0;
        std::lock_guard l{mut};
        if (logs.size() == max_logs)
        {
            logs.pop_front();
        }
        logs.push_back(std::to_string(i++) + ": " + text);
    }

    friend std::ostream &operator<<(std::ostream &os, const Logger &l)
    {
        std::lock_guard lock{l.mut};
        for (auto &s : l.logs)
        {
            os << s << '\n';
        }
        return os;
    }
};

#endif // LOGGER_HPP_
