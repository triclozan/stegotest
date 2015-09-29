#ifndef CTESTMEDIAN_H
#define CTESTMEDIAN_H
#include "ctest.h"

class CTestMedian : public CTest
{
    public:
        virtual void Attack(QImage& img, QByteArray& params);
};

#endif // CTESTMEDIAN_H
