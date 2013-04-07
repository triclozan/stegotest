#ifndef CALGKOH_H
#define CALGKOH_H
#include "calgorithm.h"

class CAlgKoh : public CAlgorithm
{
public:
    CAlgKoh();
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

#endif // CALGKOH_H
