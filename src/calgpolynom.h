#ifndef CALGPOLYNOM_H
#define CALGPOLYNOM_H
#include "calgorithm.h"

class CAlgPolynom : public CAlgorithm
{
public:
    CAlgPolynom();
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
    double fi(int i, double x);
    int mode, ch, spectrum;
    int L;
};
#endif // CALGPOLYNOM_H
