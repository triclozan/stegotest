#ifndef CALGBENGAM_H
#define CALGBENGAM_H
#include "calgorithm.h"

class CAlgBengam : public CAlgorithm
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

#endif // CALGBENGAM_H
