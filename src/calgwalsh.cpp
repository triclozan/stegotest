#include "calgwalsh.h"
#include "dct.h"
#include <QTime>
#include <math.h>
#include "util.h"

CAlgWalsh::CAlgWalsh() : CAlgMidCoeff() {
}

int CAlgWalsh::Lc[8] = {1,3,5,7,9,11,13,15};
int CAlgWalsh::walshMatrix[16][16] = {
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1, 1, -1},
            {1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1},
            {1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1, 1, -1, -1, 1},
            {1, 1, 1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1},
            {1, -1, 1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1},
            {1, 1, -1, -1, -1, -1, 1, 1, 1, 1, -1, -1, -1, -1, 1, 1},
            {1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1, 1, -1, 1, 1, -1},
            {1, 1, 1, 1, 1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1},
            {1, -1, 1, -1, 1, -1, 1, -1, -1, 1, -1, 1, -1, 1, -1, 1},
            {1, 1, -1, -1, 1, 1, -1, -1, -1, -1, 1, 1, -1, -1, 1, 1},
            {1, -1, -1, 1, 1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1},
            {1, 1, 1, 1, -1, -1, -1, -1, -1, -1, -1, -1, 1, 1, 1, 1},
            {1, -1, 1, -1, -1, 1, -1, 1, -1, 1, -1, 1, 1, -1, 1, -1},
            {1, 1, -1, -1, -1, -1, 1, 1, -1, -1, 1, 1, 1, 1, -1, -1},
            {1, -1, -1, 1, -1, 1, 1, -1, -1, 1, 1, -1, 1, -1, -1, 1}
            };

double CAlgWalsh::fi(int i, double x) {
    int col = int(floor(x / (1.0 / 16)));
    return CAlgWalsh::walshMatrix[i][col];
}

void CAlgWalsh::GenerateWM(double* encData, int Nmid, QBitArray bits)
{
    int cc = Nmid * 8 / bits.size();
    int cn = bits.size() / 8;
    double cstep = 1 / double(cc);
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

void CAlgWalsh::ExtractWM(double *data, double *mid, int Nmid, int size)
{
    int cc = Nmid * 8 / size;
    int cn = size / 8;
    double cstep = 1 / double(cc);
    for (int i=0; i<cn; i++) {
        for (int k=0; k<8; k++) {
            double res = 0;
            for (int j=0; j<cc; j++) {
                res += cstep * fi(Lc[k], cstep*j) * mid[i*cc + j];
            }
            if (res <= this->a) {
                data[i*8 + k]--;
            }
            else {
                data[i*8 + k]++;
            }
        }
    }
}

void CAlgWalsh::ExtractExtWM(double *data, double *mid, int Nmid, int size)
{
    int cc = Nmid * 8 / size;
    int cn = size / 8;
    double cstep = 1 / double(cc);
    QScopedArrayPointer<double> res(new double [size]); // input matrix
    double res_min = 100, res_max = -100;

    for (int i=0; i<cn; i++) {
        for (int k=0; k<8; k++) {
            res[i * 8 + k] = 0;
            double x = -0.99;
            for (int j=0; j<cc; j++) {
                res[i * 8 + k] += cstep * fi(Lc[k], cstep*j) * mid[i*cc + j];
            }
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
