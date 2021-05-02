#if !defined(PARSE_ARGS_HPP_)
#define PARSE_ARGS_HPP_

#include <chrono>
#include <variant>
#include <vector>

using namespace std;
using namespace chrono;

enum class Speed
{
    fast,
    medium,
    slow
};

class Settings
{
private:
    seconds duracionSimulacion;
    variant<Speed, double> speed;
    unsigned cantidadFilosofos;
    seconds maxThinkingTime;
    seconds maxEatingTime;

public:
    friend Settings parseArgs(const vector<const char *> &);

    unsigned getCantidadFilosofos() const { return cantidadFilosofos; }
    seconds getDuracionSimulacion() const { return duracionSimulacion; }
    seconds getMaxThinkingTime() const { return maxThinkingTime; }
    seconds getMaxEatingTime() const { return maxEatingTime; }

    double getSpeedFactor() const
    {
        if (holds_alternative<Speed>(speed))
        {
            switch (get<Speed>(speed))
            {
            case Speed::fast:
                return 0.5;
            case Speed::medium:
                return 1.0;
            case Speed::slow:
                return 2.0;
            }
        }

        return get<double>(speed);
    }
};

Settings parseArgs(const vector<const char *> &args);

#endif // PARSE_ARGS_HPP_
