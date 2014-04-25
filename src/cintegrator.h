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
        virtual double meshStep (int i, double base);
    };
    class TestFunctor : public Functor {
    public:
        virtual double operator() (int i, double x);
    };
    static double Rectangle(double start, double step, int count, Functor& f);
    static double Trapezium(double start, double step, int count, Functor& f);
    static double Simpson(double start, double step, int count, Functor& f);
    static double Integrate(double start, double step, int count, Functor& f, const QString &method);

    static void test();

    CIntegrator();
};

#endif // CINTEGRATOR_H
