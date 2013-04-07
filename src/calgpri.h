#ifndef CALGPRI_H
#define CALGPRI_H
#include "calgorithm.h"
class CAlgPRI : public CAlgorithm
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

#endif // CALGPRI_H
