#include "calgconvolution.h"
#include "dct.h"
#include <QTime>
#include <math.h>
#include "util.h"

int CAlgConvolution::Lc[8] = {1,2,3,4,5,6,7,8};
double CAlgConvolution::fi(int i, double x) {
    return sqrt(2) * sin(M_PI * i * x);
}

CAlgConvolution::CAlgConvolution() : CAlgMidCoeff() {
}

CAlgConvolution::Functor::Functor (int k, int start, double *mid):
    CAlgMidCoeff::Functor::Functor(k, start, mid)
{

}

double CAlgConvolution::Functor::operator() (int i, double x) {
    return fi(Lc[k], x) * mid[start + i];
}

void CAlgConvolution::GenerateWM(double* encData, int Nmid, QBitArray bits)
{
    int cc = Nmid * 8 / bits.size();
    int cn = bits.size() / 8;
    double cstep = 2 / double(cc);

    for (int i = 0; i < cn; i++) {
        int k[8];
        for (int l=0; l<8; l++) {
            k[l] = bits[i*8 + l];
        }
        for (int j=0; j<cc; j++) {
            double res = 0;
            for (int l=0; l<8; l++) {
                res += k[l] * fi(Lc[l], cstep * j);
            }
            encData[i * cc + j] = res;
        }
    }
}

void CAlgConvolution::ExtractWM(double *data, double *mid, int Nmid, int size)
{
    int cc = Nmid * 8 / size;
    int cn = size / 8;
    double cstep = 2 / double(cc);
    Functor f(1, 1, mid);
    for (int i=0; i<cn; i++) {
        f.setStart(i * cc);
        for (int k=0; k<8; k++) {
            f.setK(k);
            double res = CIntegrator::Integrate(0, cstep, cc, f, intMethod);
            if (res <= this->a) {
                data[i*8 + k]--;
            }
            else {
                data[i*8 + k]++;
            }
        }
    }
}

void CAlgConvolution::ExtractExtWM(double *data, double *mid, int Nmid, int size)
{
    int cc = Nmid * 8 / size;
    int cn = size / 8;
    double cstep = 2 / double(cc);
    Functor f(1, 1, mid);
    QScopedArrayPointer<double> res(new double [size]); // input matrix
    double res_min = 100, res_max = -100;

    for (int i=0; i<cn; i++) {
        f.setStart(i * cc);
        for (int k=0; k<8; k++) {
            f.setK(k);
            res[i * 8 + k] = CIntegrator::Integrate(0, cstep, cc, f, intMethod);
            if (res_min > res[i * 8 + k]) {
                res_min = res[i * 8 + k];
            }
            if (res_max < res[i * 8 + k]) {
                res_max = res[i * 8 + k];
            }
        }
    }
    double low_area = this->a - res_min;
    double high_area = res_max - this->a;
    for (int i=0; i<cn; i++) {
        for (int k=0; k<8; k++) {
            if (res[i * 8 + k] <= this->a) {
                data[i*8 + k] += 0.1 * ((res[i * 8 + k] - res_min) / low_area);
            }
            else {
                data[i*8 + k] += 1 - 0.9 * ((res_max - res[i * 8 + k]) / high_area);
            }
        }
    }
}
