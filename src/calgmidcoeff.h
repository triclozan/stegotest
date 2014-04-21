#ifndef CALGMIDCOEFF_H
#define CALGMIDCOEFF_H
#include "calgorithm.h"
#include "cintegrator.h"
#include <QScopedArrayPointer>
#include <QSharedPointer>

class CAlgMidCoeff : public CAlgorithm
{
public:
    CAlgMidCoeff();
    virtual void Hide(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void Restore(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void Restore(QImage& container, QVector<double> &watermark, QByteArray& key);
    virtual void GenKey(QByteArray& data);
    virtual void SetParams(QString& params);
    virtual void GenerateWM(double *encData, int Nmid, QBitArray bits);
    virtual void ExtractWM(double *data, double *mid, int Nmid, int size);
    virtual void ExtractExtWM(double *data, double *mid, int Nmid, int size);

    class Functor : public CIntegrator::Functor {
    protected:
        int k, start;
        double *mid;
    public:
        Functor (int k, int start, double* mid);
        virtual double operator() (int i, double x) = 0;

        int getStart() const;
        void setStart(int value);
        int getK() const;
        void setK(int value);
    };

    struct keyStruct {
        int length;
        int seed;
    };
    double a, gamma;
    int mode, ch, spectrum;
    QString intMethod;
};
#endif // CALGPOLYNOM_H
