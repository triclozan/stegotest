#ifndef CALGLSB_H
#define CALGLSB_H
#include "calgorithm.h"

class CAlgLSB : public CAlgorithm
{
public:
    virtual void Hide(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void Restore(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void GenKey(QByteArray& data);
};

#endif // CALGLSB_H
