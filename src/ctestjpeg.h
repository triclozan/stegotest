#ifndef CTESTJPEG_H
#define CTESTJPEG_H
#include "ctest.h"

class CTestJpeg : public CTest
{
    public:
        //virtual void Test(CAlgorithm* alg, QByteArray& params, QByteArray& alg_params, sResults& result, const QString& container_name, const QString& watermark_name);
        virtual void Attack(QImage& img, QByteArray& params);
};

#endif // CTESTJPEG_H
