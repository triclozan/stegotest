#include "calgpolynom.h"
#include "dct.h"
#include <QTime>
#include <math.h>
#include "util.h"

CAlgPolynom::CAlgPolynom() : CAlgMidCoeff() {

}

int CAlgPolynom::Lc[8] = {1,2,3,4,5,6,7,8};
double CAlgPolynom::fi(int i, double x) {
    return cos(i*acos(x));
}

int CAlgPolynom::Functor::getStart() const
{
    return start;
}

void CAlgPolynom::Functor::setStart(int value)
{
    start = value;
}

int CAlgPolynom::Functor::getK() const
{
    return k;
}

void CAlgPolynom::Functor::setK(int value)
{
    k = value;
}

CAlgPolynom::Functor::Functor (int k, int start, double *mid)
{
    this->k = k;
    this->start = start;
    this->mid = mid;
}

double CAlgPolynom::Functor::operator() (int i, double x) {
    return fi(Lc[k], x) * mid[start + i] * (1.0 / sqrt(1 - x*x));
}

void CAlgPolynom::GenerateWM(double* encData, int Nmid, QBitArray bits)
{
    qDebug() << "gen";
    int cc = Nmid * 8 / bits.size();
    int cn = bits.size() / 8;
    double cstep = 1.99 / double(cc);

    for (int i = 0; i < cn; i++) {
        int k[8];
        for (int l=0; l<8; l++) {
            k[l] = bits[i*8 + l];
            bits[i*8 + l];
        }
        double x = -0.995;

        for (int j=0; j<cc; j++) {
            double res = 0;
            for (int l=0; l<8; l++) {
                res += k[l] * fi(Lc[l], x);
            }
            x = x + cstep;
            encData[i * cc + j] = res;
        }
    }
}

void CAlgPolynom::ExtractWM(double *data, double *mid, int Nmid, int size)
{
    qDebug() << "extract";
    int cc = Nmid * 8 / size;
    int cn = size / 8;
    double cstep = 1.99 / double(cc);
    Functor f(1, 1, mid);
    for (int i=0; i<cn; i++) {
        f.setStart(i * cc);
        for (int k=0; k<8; k++) {
            f.setK(k);
            double res = CIntegrator::Integrate(-0.995, cstep, cc, f, intMethod);
            if (res <= this->a) {
                data[i*8 + k]--;
            }
            else {
                data[i*8 + k]++;
            }
        }
    }
}

void CAlgPolynom::ExtractExtWM(double *data, double *mid, int Nmid, int size)
{
    int cc = Nmid * 8 / size;
    int cn = size / 8;
    double cstep = 1.98 / double(cc);
    QScopedArrayPointer<double> res(new double [size]); // input matrix
    double res_min = 100, res_max = -100;
    Functor f(1, 1, mid);
    for (int i=0; i<cn; i++) {
        f.setStart(i * cc);
        for (int k=0; k<8; k++) {
            f.setK(k);
            res[i * 8 + k] = CIntegrator::Integrate(-0.99, cstep, cc, f, intMethod);
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
