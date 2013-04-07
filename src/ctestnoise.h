#ifndef CTESTNOISE_H
#define CTESTNOISE_H
#include "ctest.h"

class CTestNoise : public CTest
{
public:
    virtual void Test(CAlgorithm* alg, QByteArray& params, QByteArray& alg_params, sResults& result, const QString& container_name, const QString& watermark_name);

};

#endif // CTESTNOISE_H
