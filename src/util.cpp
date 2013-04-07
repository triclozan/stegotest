#include "util.h"
#include <QStringList>
#include <QColor>
#include <QByteArray>
unsigned int YCbCrtoRGB(double y, int cr, int cb)
{
    QColor color;
    double R = y + cr + (cr >> 2) + (cr >> 3) + (cr >> 5);
    double G = y - ((cb >> 2) + (cb >> 4) + (cb >> 5)) - ((cr >> 1) + (cr >> 3) + (cr >> 4) + (cr >> 5));
    double B = y + cb + (cb >> 1) + (cb >> 2) + (cb >> 6);
    color.setRed(PIXELRANGE(ROUND(R)));
    color.setGreen(PIXELRANGE(ROUND(G)));
    color.setBlue(PIXELRANGE(ROUND(B)));
    return color.rgb();
}

void RGBtoYCbCr(unsigned int value, double& y, int& cr, int& cb)
{
    cb = -0.169*RED(value) - 0.331*GREEN(value) + 0.499*BLUE(value);
    cr = 0.499*RED(value) - 0.418*GREEN(value) - 0.0813*BLUE(value);
    y = 0.299*RED(value) + 0.587*GREEN(value) + 0.114*BLUE(value);
}

double RGBtoY(unsigned int value)
{
    return 0.299*RED(value) + 0.587*GREEN(value) + 0.114*BLUE(value);
}
