#include "cintegrator.h"
#include <QDebug>

CIntegrator::CIntegrator()
{
}

double CIntegrator::Rectangle(double start, double step, int count, Functor& f)
{
    double x = start;
    double sum = 0;
    for (int i=0; i<count; i++) {
        sum += f(i, x) * step;
        x += step;
    }
    return sum;
}

double CIntegrator::Trapezium(double start, double step, int count, Functor& f)
{
    double x = start;
    double sum = 0;
    for (int i=0; i<count; i++) {
        sum += (f(i + 1, x + step) + f(i, x)) / 2 * step;
        x += step;
    }
    return sum;}

double CIntegrator::Simpson(double start, double step, int count, Functor& f)
{
    double x = start;
    double C = step / 3;
    double step2 = step * 2;
    double f1 = 0, f2 = 0;
    for (int i = 0; i < count - 2; i += 2) {
        f1 += f(i + 1, x + step);
        f2 += f(i + 2, x + step2);
        x += step2;
    }
    f1 += f(count - 1, x + step);
    return C * (f(0, start) + 4 * f1 + 2 * f2 + f(count, x + step2));
}

double CIntegrator::Integrate(double start, double step, int count, Functor& f, const QString& method)
{
    if (method.compare("rectangle") == 0) {
        return Rectangle(start, step, count, f);
    } else if (method.compare("trapezium") == 0) {
        return Trapezium(start, step, count, f);
    } else if (method.compare("simpson") == 0) {
        return Simpson(start, step, count, f);
    }
}
