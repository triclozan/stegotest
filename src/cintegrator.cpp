#include "cintegrator.h"
#include <cmath>
#include <QDebug>

CIntegrator::CIntegrator()
{
}

double CIntegrator::Functor::meshStep (int i, double base)
{
    return base;
}

double CIntegrator::TestFunctor::operator ()(int i, double x) {
    return sin(0.5 * x);
}

double CIntegrator::Rectangle(double start, double step, int count, Functor& f)
{
    double x = start;
    double sum = 0;
    for (int i=0; i<count - 1; i++) {
        sum += f(i, x)* step;
        x += step;
    }
    return sum;
}

/*double CIntegrator::Nonuniform(double start, double step, int count, Functor& f)
{
    double x = start;
    double sum = 0;
    for (int i=0; i<count; i++) {
        sum += f(i, x) * step;
        x += step;
    }
    return sum;
}*/

double CIntegrator::Trapezium(double start, double step, int count, Functor& f)
{
    //qDebug() << "trapezium!!!";
    double x = start;
    double sum = 0;
    for (int i=0; i<count - 1; i++) {
        sum += (f(i + 1, x + step) + f(i, x)) / 2 * step;
        x += step;
    }
    return sum;
}

double CIntegrator::Simpson(double start, double step, int count, Functor& f)
{
    double x = start;
    double C = step / 3;
    double step2 = step * 2;
    count-=3;
    double f1 = 0, f2 = 0;
    for (int i = 0; i < count - 2; i += 2) {
        f1 += f(i + 1, x + step);
        f2 += f(i + 2, x + step2);
        x += step2;
    }
    f1 += f(count - 1, x + step);
    return C * (f(0, start) + 4 * f1 + 2 * f2 + f(count, x + step2)) + (f(count - 1, x + step * 2) + f(count, x + step * 3)) / 2 * step;
}

double CIntegrator::Simpson2(double start, double step, int count, Functor& f) //count = 2N + 1
{
    bool tail = false;    
    if (!(count % 2)) {
        tail = true;
        count--;
    }
    double x = start;
    double C = step / 3;
    double step2 = step * 2;
    double f1 = 0, f2 = 0;
    for (int i = 0; i < count - 4; i += 2) { // 0, 2, 4, ... , 2N - 4
        f1 += f(i + 1, x + step); // 1 ... 2N - 3
        f2 += f(i + 2, x + step2); // 2 ... 2N - 2
        x += step2;
    }
    f1 += f(count - 2, x + step); // 1 ... 2N - 1
    double res = C * (f(0, start) + 4 * f1 + 2 * f2 + f(count - 1, x + step2)); // 0 ... 2N
    if (tail) {
        res += (f(count - 1, x + step * 2) + f(count, x + step * 3)) / 2 * step;
    }
    return res;
}

double CIntegrator::Integrate(double start, double step, int count, Functor& f, const QString& method)
{
    if (method.compare("rectangle") == 0) {
        return Rectangle(start, step, count, f);
    } else if (method.compare("trapezium") == 0) {
        return Trapezium(start, step, count, f);
    } else if (method.compare("simpson") == 0) {
        return Simpson(start, step, count, f);
    } else if (method.compare("simpson2") == 0) {
        return Simpson2(start, step, count, f);
    }
}

void CIntegrator::test() {
    TestFunctor f;
    double start = 0;
    double end = 1.3;
    int count = 25;
    double step = (end - start) / (count);
    qDebug() << CIntegrator::Integrate(start, step, count + 1, f, "rectangle");
    qDebug() << CIntegrator::Integrate(start, step, count + 1, f, "trapezium");
    qDebug() << CIntegrator::Integrate(start, step, count + 1, f, "simpson");
    qDebug() << CIntegrator::Integrate(start, step, count + 1, f, "simpson2");
}
