#include "parseArgs.hpp"
#include <iostream>
#include <sstream>

istream &operator>>(istream &is, Speed &s)
{
    string str;
    is >> str;
    if (str == "fast")
    {
        s = Speed::fast;
    }
    else if (str == "medium")
    {
        s = Speed::medium;
    }
    else if (str == "slow")
    {
        s = Speed::slow;
    }
    else
    {
        is.setstate(istream::badbit);
    }
    return is;
}

Settings parseArgs(const vector<const char *> &args)
{
    constexpr auto ARG_COUNT = 5U;
    if (args.size() < ARG_COUNT)
    {
        cerr << "Opciones inválidas.\n"
             << "Debe ingresar: <cantidadFilosofos> <velocidad: fast | medium "
                "| slow> <duracionSimulacion> <maxThinkingTime> "
                "<maxEatingTime>\n";
        exit(EXIT_FAILURE);
    }

    Settings sett;

    if (!(stringstream(args[0]) >> sett.cantidadFilosofos))
    {
        cerr << "La cantidad de fiósofos debe ser un número entero positivo\n";
        exit(EXIT_FAILURE);
    }

    if (double d; stringstream(args[1]) >> d)
    {
        sett.speed = d;
    }
    else if (Speed s; stringstream(args[1]) >> s)
    {
        sett.speed = s;
    }
    else
    {
        cerr << "La velocidad dee ser fast, medium, slow o un número.\n";
        exit(EXIT_FAILURE);
    }

    unsigned long aux;

    if (!(stringstream(args[2]) >> aux))
    {
        cerr << "La duración de la simulación debe ser un número entero "
                "positivo\n";
        exit(EXIT_FAILURE);
    }
    sett.duracionSimulacion = seconds{aux};

    if (!(stringstream(args[3]) >> aux))
    {
        cerr << "La cantidad de fiósofos debe ser un número entero "
                "positivo\n";
        exit(EXIT_FAILURE);
    }
    sett.maxThinkingTime = seconds{aux};

    if (!(stringstream(args[4]) >> aux))
    {
        cerr << "La cantidad de fiósofos debe ser un número entero "
                "positivo\n";
        exit(EXIT_FAILURE);
    }
    sett.maxEatingTime = seconds{aux};

    return sett;
}
