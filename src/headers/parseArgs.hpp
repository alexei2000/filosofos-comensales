#if !defined(PARSE_ARGS_HPP_)
#define PARSE_ARGS_HPP_

#include <chrono>
#include <vector>

using namespace std;
using namespace chrono;

enum class Speed
{
    fast,
    medium,
    slow
};

struct Settings
{
    milliseconds duracionSimulacion;
    Speed speed;
    unsigned cantidadFilosofos;
    milliseconds maxThinkingTime;
    milliseconds maxEatingTime;
};

Settings parseArgs(const vector<const char *> &args);

#endif // PARSE_ARGS_HPP_
