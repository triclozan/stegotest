#ifndef CALGCOMBINE_H
#define CALGCOMBINE_H
#include "calgkoh.h"
#include "calgkutter.h"
#include "calgfridrich2.h"

class CAlgCombine : public CAlgorithm
{
public:
    CAlgCombine();
    ~CAlgCombine();
    virtual void Hide(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void Restore(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void GenKey(QByteArray& data);
    virtual void SetParams(QString& params);
private:
    double koh_tresh, power, alpha, gamma;
    int ch, which, mode, mult;
    CAlgorithm* koh;
    CAlgorithm* kutter;
    CAlgorithm* fridrich;
};

#endif // CALGCOMBINE_H
