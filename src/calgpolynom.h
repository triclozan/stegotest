#ifndef CALGPOLYNOM_H
#define CALGPOLYNOM_H
#include "calgmidcoeff.h"
#include "cintegrator.h"

class CAlgPolynom : public CAlgMidCoeff
{
public:
    CAlgPolynom();
    virtual void GenerateWM(double *encData, int Nmid, QBitArray bits);
    virtual void ExtractWM(double *data, double *mid, int Nmid, int size);
    virtual void ExtractExtWM(double *data, double *mid, int Nmid, int size);

    static int Lc[8];
    static double fi(int i, double x);
    class Functor : public CAlgMidCoeff::Functor {
        int numPoints;
        double length;
    public:
        Functor (int k, int start, double* mid);
        virtual double operator() (int i, double x);
        virtual double meshStep (int i, double base);
        int getNumPoints() const;
        void setNumPoints(int value);
        double getLength() const;
        void setLength(double value);
    };
};
#endif // CALGPOLYNOM_H
