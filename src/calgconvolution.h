#ifndef CALGCONVOLUTION_H
#define CALGCONVOLUTION_H
#include "calgmidcoeff.h"

class CAlgConvolution : public CAlgMidCoeff
{
public:
    CAlgConvolution();
    virtual void GenerateWM(double *encData, int Nmid, QBitArray bits);
    virtual void ExtractWM(double *data, double *mid, int Nmid, int size);
    virtual void ExtractExtWM(double *data, double *mid, int Nmid, int size);

    static int Lc[8];
    static double fi(int i, double x);

    class Functor : public CAlgMidCoeff::Functor {
    public:
        Functor (int k, int start, double* mid);
        virtual double operator() (int i, double x);
    };
};

#endif // CALGCONVOLUTION_H
