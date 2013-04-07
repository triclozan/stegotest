#ifndef CTESTKOH_H
#define CTESTKOH_H
#include "ctest.h"

class CTestKoh : public CTest
{
    public:
        QImage Test(const QString& container_name, const QString& watermark_name);
    //virtual void Test(CAlgorithm* alg, QByteArray& params, QByteArray& alg_params, sResults& result, const QString& container_name, const QString& watermark_name);
        virtual void Attack(QImage& img, QByteArray& params);
};

#endif // CTESTKOH_H
