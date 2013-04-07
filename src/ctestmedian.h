#ifndef CTESTMEDIAN_H
#define CTESTMEDIAN_H
#include "ctest.h"

class CTestMedian : public CTest
{
    public:
        //virtual void Test(CAlgorithm* alg, QByteArray& params, QByteArray& alg_params, sResults& result, const QString& container_name, const QString& watermark_name);
        virtual void Attack(QImage& img, QByteArray& params);
};

#endif // CTESTMEDIAN_H
