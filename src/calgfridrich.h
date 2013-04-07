#ifndef CALGFRIDRICH_H
#define CALGFRIDRICH_H
#include "calgorithm.h"

class CAlgFridrich : public CAlgorithm
{
public:
    CAlgFridrich();
    virtual void Hide(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void Restore(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void GenKey(QByteArray& data);
    virtual void SetParams(QString& params);
    struct keyStruct {
        int length;
        int seed;
    };
    double a, gamma;
    int mode;
};

#endif // CALGFRIDRICH_H
