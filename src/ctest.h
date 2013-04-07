#ifndef CTEST_H
#define CTEST_H
#include <QString>
#include "calgorithm.h"
#include "cimgcompare.h"

class CTest
{
public:
    struct sResults {
        double BER;
        double ByER;
    };
    virtual void Test(CAlgorithm* alg, QString& params, QString& alg_params, QByteArray& result, const QString& container_name, const QString& watermark_name);
    void Visual(CAlgorithm* alg, QString& params, QString& alg_params, const QString& container_name, const QString* watermark_name, int wm_size);
    virtual void Attack(QImage& img, QString& params);
    void addNoise(QImage& in, QImage& out, int level);
    void addINoise(QImage& in, QImage& out, int level);
    void addBINoise(QImage& in, QImage& out, int level);
    void addGNoise(QImage& in, QImage& out, int level);
    void AMFilter(QImage& in, QImage& out, int N = 1);
    void GMFilter(QImage& in, QImage& out, int N = 1);
    void HMFilter(QImage& in, QImage& out, int N = 1);
    void MedFilter(QImage& in, QImage& out, int N = 1);
    void MPFilter(QImage& in, QImage& out, int N = 1);
    void AdMedFilter(QImage& in, QImage& out, int N = 1);
    void SetParams(QString params);
    void Sharpen(QImage& in, QImage& out, double strength = 0.1);
    virtual ~CTest();
private:
    QString params;
    int size, tsize, num;
};

#endif // CTEST_H
