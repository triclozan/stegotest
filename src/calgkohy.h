#ifndef CALGKOHY_H
#define CALGKOHY_H
#include "calgorithm.h"

class CAlgKohY : public CAlgorithm
{
public:
    virtual void Hide(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void Restore(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void GenKey(QByteArray& data);
    struct keyStruct {
        int length;
        int seed;
    };
};

#endif // CALGKOHY_H
