#ifndef CALGFRIDRICH2_H
#define CALGFRIDRICH2_H
#include "calgorithm.h"

class CAlgFridrich2 : public CAlgorithm
{
public:
    CAlgFridrich2();
    virtual void Hide(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void Restore(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void GenKey(QByteArray& data);
    virtual void SetParams(QString& params);
    struct keyStruct {
        int length;
        int seed;
    };
    double a, gamma;
    int mode, ch;
    int L;
};

#endif // CALGFRIDRICH2_H
