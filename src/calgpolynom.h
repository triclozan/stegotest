#ifndef CALGPOLYNOM_H
#define CALGPOLYNOM_H
#include "calgmidcoeff.h"

class CAlgPolynom : public CAlgMidCoeff
{
public:
    CAlgPolynom();
    virtual void GenerateWM(double *encData, int Nmid, QBitArray bits);
    virtual void ExtractWM(double *data, double *mid, int Nmid, int size);
    virtual void ExtractExtWM(double *data, double *mid, int Nmid, int size);

    static int Lc[8];
    double fi(int i, double x);
};
#endif // CALGPOLYNOM_H
