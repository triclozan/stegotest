#ifndef CINTEGRATOR_H
#define CINTEGRATOR_H

#include "util.h"
#include <QString>

class CIntegrator
{
public:
    class Functor {
    public:
        virtual double operator() (int i, double x) = 0;
    };
    static double Rectangle(double start, double step, int count, Functor& f);
    static double Trapezium(double start, double step, int count, Functor& f);
    static double Simpson(double start, double step, int count, Functor& f);
    static double Integrate(double start, double step, int count, Functor& f, const QString &method);
    CIntegrator();
};

#endif // CINTEGRATOR_H
