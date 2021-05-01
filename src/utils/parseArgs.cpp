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
        is.setstate(is.badbit);
    }
    return is;
}

Settings parseArgs(const vector<const char *> &args)
{
    if (args.size() < 5)
    {
        cerr << "Opciones inválidas.\n"
             << "Debe ingresar: <duracionSimulacion> <velocidad: fast | medium "
                "| slow> <cantidadFilosofos> "
                "<maxThinkingTime> <maxEatingTime>\n";
        exit(EXIT_FAILURE);
    }

    Settings sett;

    if (!(stringstream(args[0]) >> sett.cantidadFilosofos))
    {
        cerr << "La cantidad de fiósofos debe ser un número entero positivo\n";
        exit(EXIT_FAILURE);
    }

    if (!(stringstream(args[1]) >> sett.speed))
    {
        cerr << "La velocidad dee ser fast, medium o slow.\n";
        exit(EXIT_FAILURE);
    }

    unsigned long aux;

    if (!(stringstream(args[2]) >> aux))
    {
        cerr << "La duración de la simulación debe ser un número entero "
                "positivo\n";
        exit(EXIT_FAILURE);
    }
    sett.duracionSimulacion = milliseconds{aux};

    if (!(stringstream(args[3]) >> aux))
    {
        cerr << "La cantidad de fiósofos debe ser un número entero positivo\n";
        exit(EXIT_FAILURE);
    }
    sett.maxThinkingTime = milliseconds{aux};

    if (!(stringstream(args[4]) >> aux))
    {
        cerr << "La cantidad de fiósofos debe ser un número entero positivo\n";
        exit(EXIT_FAILURE);
    }
    sett.maxEatingTime = milliseconds{aux};

    return sett;
}
