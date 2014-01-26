#ifndef CALGWALSH_H
#define CALGWALSH_H
#include "calgorithm.h"

class CAlgWalsh : public CAlgorithm
{
public:
    CAlgWalsh();
    virtual void Hide(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void Restore(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void Restore(QImage& container, QVector<double> &watermark, QByteArray& key);
    virtual void GenKey(QByteArray& data);
    virtual void SetParams(QString& params);
    struct keyStruct {
        int length;
        int seed;
    };
    double a, gamma;
    static int Lc[8];
    static int walshMatrix[16][16];
    double fi(int i, double x);
    int mode, ch, spectrum;
    int L;
};

#endif // CALGWALSH_H
