#ifndef CTESTNOISE_H
#define CTESTNOISE_H
#include "ctest.h"

class CTestNoise : public CTest
{
public:
    virtual void Attack(QImage& img, QByteArray& params);
};

#endif // CTESTNOISE_H
