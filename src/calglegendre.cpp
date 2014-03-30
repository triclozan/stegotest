#include "calglegendre.h"
#include "dct.h"
#include <QTime>
#include <math.h>
#include "util.h"

CAlgLegendre::CAlgLegendre() : CAlgMidCoeff() {
}

int CAlgLegendre::Lc[8] = {1,2,3,4,5,6,7,8};
double CAlgLegendre::fi(int i, double x) {
    switch(i) {
    case 1: return x;
    case 2: return (3 * x * x - 1) / 2;
    case 3: return (5 * pow(x,3) - 3 * x) / 2;
    case 4: return (35 * pow(x,4) - 30 * x * x + 3) / 8;
    case 5: return (63 * pow(x,5) - 70 * pow(x,3) + 15 * x) / 8;
    case 6: return (231 * pow(x,6) - 315 * pow(x,4) + 105 * x*x - 5) / 16;
    case 7: return (429 * pow(x,7) - 693 * pow(x,5) + 315 * pow(x,3) - 35 * x) / 16;
    case 8: return (6435 * pow(x,8) - 12012 * pow(x,6) + 6930 * pow(x,4) - 1260 * pow(x,2) + 35) / 128;
    }
}

void CAlgLegendre::GenerateWM(double* encData, int Nmid, QBitArray bits)
{
    int cc = Nmid * 8 / bits.size();
    int cn = bits.size() / 8;
    double cstep = 2 / double(cc);
    for (int i = 0; i < cn; i++) {
        int k[8];
        for (int l=0; l<8; l++) {
            k[l] = bits[i*8 + l];
            bits[i*8 + l];
        }
        double x = -1;
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

void CAlgLegendre::ExtractWM(double *data, double *mid, int Nmid, int size)
{
    int cc = Nmid * 8 / size;
    int cn = size / 8;
    double cstep = 2 / double(cc);
    for (int i=0; i<cn; i++) {
        for (int k=0; k<8; k++) {
            double res = 0;
            double x = -1;
            for (int j=0; j<cc; j++) {
                res += cstep * fi(Lc[k], x) * mid[i*cc + j];
                x = x + cstep;
            }
            res *= ((2 * Lc[k] + 1) / 2.0);
            if (res <= this->a) {
                data[i*8 + k]--;
            }
            else {
                data[i*8 + k]++;
            }
        }
    }
}

void CAlgLegendre::ExtractExtWM(double *data, double *mid, int Nmid, int size)
{
    int cc = Nmid * 8 / size;
    int cn = size / 8;
    double cstep = 2 / double(cc);
    QScopedArrayPointer<double> res(new double [size]); // input matrix
    double res_min = 100, res_max = -100;

    for (int i=0; i<cn; i++) {
        for (int k=0; k<8; k++) {
            res[i * 8 + k] = 0;
            double x = -1;
            for (int j=0; j<cc; j++) {
                res[i * 8 + k] += cstep * fi(Lc[k], x) * mid[i*cc + j];
                x = x + cstep;
            }
            res[i * 8 + k] *= ((2 * Lc[k] + 1) / 2.0);
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
