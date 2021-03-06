#ifndef CKUTTER_H
#define CKUTTER_H
#include "calgorithm.h"
class CAlgKutter : public CAlgorithm
{
public:
    CAlgKutter();
    virtual void Hide(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void Restore(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void GenKey(QByteArray& data);
    virtual void SetParams(QString& params);
    struct keyStruct {
        int length;
        int seed;
    };
    double v;
    int r, len;
    int ch;
};

#endif // CKUTTER_H
