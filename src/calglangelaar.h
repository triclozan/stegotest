#ifndef CALGLANGELAAR_H
#define CALGLANGELAAR_H
#include "calgorithm.h"

class CAlgLangelaar : public CAlgorithm
{
public:
    CAlgLangelaar();
    virtual void Hide(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void Restore(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void GenKey(QByteArray& data);
    virtual void SetParams(QString& params);
    struct keyStruct {
        int length;
        int seed;
    };
private:
    double tresh;
    int ch;
};

#endif // CALGLANGELAAR_H
