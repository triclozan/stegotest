#ifndef CALGWALSH_H
#define CALGWALSH_H
#include "calgmidcoeff.h"

class CAlgWalsh : public CAlgMidCoeff
{
public:
    CAlgWalsh();
    virtual void GenerateWM(double *encData, int Nmid, QBitArray bits);
    virtual void ExtractWM(double *data, double *mid, int Nmid, int size);
    virtual void ExtractExtWM(double *data, double *mid, int Nmid, int size);

    static int Lc[8];
    double fi(int i, double x);
    static int walshMatrix[16][16];
};

#endif // CALGWALSH_H
