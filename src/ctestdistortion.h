#ifndef CTESTDISTORTION_H
#define CTESTDISTORTION_H
#include <QString>
#include "calgorithm.h"
#include "ctest.h"
class CTestDistortion : public CTest
{
public:
    CTestDistortion();
    void SetParams(QString params);
    virtual void Test(CAlgorithm* alg, QString& params, QString& alg_params, QByteArray& result, const QString& container_name, const QString& watermark_name, int size = 0);
private:
    int ch;
};

#endif // CTESTDISTORTION_H
