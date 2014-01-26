#include "calgpolynom.h"
#include "dct.h"
#include <QTime>
#include <math.h>
#include "util.h"

int Lc[8] = {1,2,3,4,5,6,7,8};
int CAlgPolynom::Lc[8] = {1,2,3,4,5,6,7,8};
double CAlgPolynom::fi(int i, double x) {
    return cos(i*acos(x));
}

CAlgPolynom::CAlgPolynom() {
    a = 0.35;
    gamma = 1;
    mode = 3;
    ch = 3;
    L = 20;
    spectrum = 0;
}

void CAlgPolynom::Hide(QImage& container, QByteArray& watermark, QByteArray& key)
{
    qDebug() << a << " " << gamma;
    qDebug() << "L: " << L;
    keyStruct keyData = *(keyStruct*)(key.constData());
    //qDebug() << keyData.seed;
    qsrand(keyData.seed);
    QBitArray bits = byteToBit(watermark);

    int bsize = bits.size();
    int k = 0;
    int l = 0;
    int m = 0;
    int X = container.width();
    int Y = container.height();
    int Nbx = 2;
    int Nby = 2;
    int Nb = Nbx*Nby;
    int Xb = X / Nbx;
    //double gamma = 1;
    int Yb = Y / Nby;
    //int L = 20;
    double* A = new double [Xb*Yb];
    double* B = new double [Xb*Yb];
    int Nmid = Xb*Yb*0.3;//4080;
    double** Spr = new double* [Nb];
    double* encData = new double[Nmid];
    for (int i=0; i<Nb; i++) {
        Spr[i] = new double [Nmid];
    }

    double** eps = new double* [bsize];
    double* eta = new double[bsize];
    int* start = new int[bsize];
    for (int i=0; i<bsize; i++) {
        eps[i] = new double [Nmid + 16];
        for (int j=0; j<Nmid+16; j++) {
            eps[i][j] = qrand() / (double)RAND_MAX;
        }
        start[i] = rand() % 6;
        if (bits[i]) {
            start[i] += 9;
        }
    }

    for (int b=0; b<Nb; b++) {
        int cf1 = b * bsize / Nb;
        int cf2 = bsize / Nb;
        for (int j=0; j<Nmid; j++) {
            double sum = 0;
            for (int i=0; i<cf2; i++) {
                sum += eps[cf1 + i][j + start[cf1 + i]];
            }
            Spr[b][j] = (sum - (cf2) / 2.0) / sqrt((cf2) / 12.0);
            //qDebug() << Spr[b][j];
        }
    }

    int** Cb = new int* [Yb];
    int** Cr = new int* [Yb];
    double cnorm;
    for (int j=0; j<Yb; j++) {
        /*A[j] = new double[Xb];
        B[j] = new double[Xb];*/
        Cb[j] = new int[Xb];
        Cr[j] = new int[Xb];
    }
    double y, cb, cr;
    double mean = 0;
    double stddev = 0;
    DCT Dct;
    for (int b=0; b<Nb; b++) {
        for (int i=0; i<Nmid; i++) {
            mean += Spr[b][i];
            stddev += Spr[b][i]*Spr[b][i];
        }
        mean /= (Nmid);
        stddev /= (Nmid);
        stddev -= mean*mean;
        stddev = sqrt(stddev);
        qDebug() << "stats:" << mean << " " << stddev;
    }
    //return;

    //double a = 0.3;
    double a2 = (1 + a) / (1 - a);
    double tau[3000];
    tau[0] = 1;
    int tau_max = 1;
    for (int i=1; i<3000; i++) {
        tau_max++;
        tau[i] = a2 * tau[i-1];
        if (tau[i] > 1000) {
            break;
        }
        //qDebug() << tau[i];
    }
    //qDebug() << tau_max;
    int H = L;
    int count = L + 1;
    while (count < bits.size()) {
        H++;
        count += H + 1;
    }
    int Lm = Xb;
    int Hm = Lm;
    int j = 1;
    int countm = Xb;
    while (countm < Nmid) {
        Hm++;
        Lm--;
        countm += 2*(Xb-j);
        j++;
    }

    int cc = Nmid * 8 / bits.size();
    int cn = bits.size() / 8;
    double cstep = 1.98 / double(cc);
    qDebug() << "COEFF: " << cc << " " << cn << " " << cstep;
    for (int i = 0; i < cn; i++) {
        int k[8];
        for (int l=0; l<8; l++) {
            k[l] = bits[i*8 + l];
            bits[i*8 + l];
        }
        double x = -0.99;
        //double x = -1;
        for (int j=0; j<cc; j++) {
            double res = 0;
            for (int l=0; l<8; l++) {
                res += k[l] * fi(Lc[l], x);
            }
            x = x + cstep;
            encData[i * cc + j] = res;
            //qDebug() << x << " " << res;
        }
    }

    for (int b=0; b<Nb; b++) {
        int by = Yb * (b / Nbx);
        int bx = Xb * (b % Nbx);

        for (int i=0; i<Xb; i++) {
            for (int j=0; j<Yb; j++) {
                double v;
                int cb, cr;
                switch(ch) {
                case channels::BLUE:
                    v = BLUE(container.pixel(bx+i, by+j));
                    break;
                case channels::RED:
                    v = RED(container.pixel(bx+i, by+j));
                    break;
                case channels::GREEN:
                    v = GREEN(container.pixel(bx+i, by+j));
                    break;
                case channels::Y:
                    RGBtoYCbCr(container.pixel(bx+i, by+j), v, cr, cb);
                    break;
                }
                mean += v;
                stddev += v*v;
            }
        }
        mean /= (Xb*Yb);
        stddev /= (Xb*Yb);
        stddev -= mean*mean;
        stddev = sqrt(stddev);
        cnorm = 1024 / (Xb*stddev);
        for (int i=0; i<Xb; i++) {
            for (int j=0; j<Yb; j++) {
                switch(ch) {
                case channels::BLUE:
                    A[i*Yb + j] = (BLUE(container.pixel(bx+i, by+j)) - mean) * cnorm;
                    break;
                case channels::RED:
                    A[i*Yb + j] = (RED(container.pixel(bx+i, by+j)) - mean) * cnorm;
                    break;
                case channels::GREEN:
                    A[i*Yb + j] = (GREEN(container.pixel(bx+i, by+j)) - mean) * cnorm;
                    break;
                case channels::Y:
                    RGBtoYCbCr(container.pixel(bx+i, by+j), A[i*Yb + j], Cr[i][j], Cb[i][j]);
                    A[i*Yb + j] = (A[i*Yb + j] - mean) * cnorm;
                    break;
                }
            }
        }

        Dct.fdct2(A, B, Xb, Yb);



        count = 0;
        countm = 0;
        double max_coeff = 0;

        double min = 10000;
        double max = -10000;
        for (int i1=0; i1<Xb; i1++) {
            for (int i2=0; i2<Yb; i2++) {
                if (Lm <= i1 + i2 + 1 && i1 + i2 + 1 <= Hm) {
                    B[i1 * Yb + i2] += encData[countm] * gamma;
                    if (encData[countm] * gamma > max) {
                        max = encData[countm] * gamma;
                    }
                    if (encData[countm] * gamma < min) {
                        min = encData[countm] * gamma;
                    }
                    countm++;
                    if (countm >= Nmid) break;
                }
            }
            if (countm >= Nmid) break;
        }
        qDebug() << "max: " << max << ", min: " << min;

        if (spectrum) {
            double valMax = 0;
            for (int i1=0; i1<Xb; i1++) {
                for (int i2=0; i2<Yb; i2++) {
                    double v = log(1 + B[i1*Yb + i2]);
                    if (v > valMax) {
                        valMax = v;
                    }
                }
            }

            for (int i1=0; i1<Xb; i1++) {
                for (int i2=0; i2<Yb; i2++) {
                    double Y = B[i1 * Yb + i2];
                    int val = PIXELRANGE(ROUND( Y / valMax * 255 ));
                    switch(ch) {
                    case channels::BLUE:
                        container.setPixel(bx+i1, by+i2, SET_BLUE(container.pixel(bx+i1, by+i2), PIXELRANGE(ROUND(Y))));
                        break;
                    case channels::RED:
                        container.setPixel(bx+i1, by+i2, SET_RED(container.pixel(bx+i1, by+i2), PIXELRANGE(ROUND(Y))));
                        break;
                    case channels::GREEN:
                        container.setPixel(bx+i1, by+i2, SET_GREEN(container.pixel(bx+i1, by+i2), PIXELRANGE(ROUND(Y))));
                        break;
                    case channels::Y:
                        container.setPixel(bx+i1, by+i2, PIXEL(val, val, val));
                        break;
                    }
                }
            }
        }
        else {
            Dct.idct2(B, A, Xb, Yb);

            for (int i1=0; i1<Xb; i1++) {
                for (int i2=0; i2<Yb; i2++) {
                    double Y = A[i1 * Yb + i2] / cnorm + mean;
                    switch(ch) {
                    case channels::BLUE:
                        container.setPixel(bx+i1, by+i2, SET_BLUE(container.pixel(bx+i1, by+i2), PIXELRANGE(ROUND(Y))));
                        break;
                    case channels::RED:
                        container.setPixel(bx+i1, by+i2, SET_RED(container.pixel(bx+i1, by+i2), PIXELRANGE(ROUND(Y))));
                        break;
                    case channels::GREEN:
                        container.setPixel(bx+i1, by+i2, SET_GREEN(container.pixel(bx+i1, by+i2), PIXELRANGE(ROUND(Y))));
                        break;
                    case channels::Y:
                        container.setPixel(bx+i1, by+i2, YCbCrtoRGB(Y, Cr[i1][i2], Cb[i1][i2]));
                        break;
                    }
                }
            }
        }
    }
    for (int i=0; i<Nb; i++) {
        delete [] Spr[i];
    }
    for (int i=0; i<bsize; i++) {
        delete [] eps[i];
    }
    for (int i=0; i<Yb; i++) {
        delete [] Cb[i];
        delete [] Cr[i];
    }
    delete [] Cb;
    delete [] Cr;
    delete [] start;
    delete [] A;
    delete [] B;
    delete [] Spr;
    delete [] eps;
    delete [] eta;
    qDebug() << "YYYYYY";
    //container.save("F:\\koh.bmp");
}

void CAlgPolynom::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
{

    qDebug() << "polynom";
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    //qDebug() << keyData.seed;
    int length = keyData.length;
    //cout << keyData.length;
    QBitArray bits;
    bits.resize(length);
    int bsize = bits.size();
    //double gamma = 1;
    int k = 0;
    int l = 0;
    int X = container.width();
    int Y = container.height();
    int Nbx = 2;
    int Nby = 2;
    int Nb = Nbx*Nby;
    int Xb = X / Nbx;
    int Yb = Y / Nby;
    int Nmid = Xb*Yb*0.3; //4080;
    //int L = 20;
    double* A = new double [Xb*Yb];
    double* B = new double [Xb*Yb];
    double* data = new double[length];
    for (int i=0; i<length; i++) {
        data[i] = 0;
    }
    double cnorm;
    double y, cb, cr;
    double mean = 0;
    double stddev = 0;
    DCT Dct;
    double* mid = new double [Nmid];
    //double a = 0.3;
    double a2 = (1 + a) / (1 - a);
    double tau[3000];
    qDebug() << "RESTORE******************";
    tau[0] = 1;
    int tau_max = 1;
    for (int i=1; i<3000; i++) {
        tau_max++;
        tau[i] = a2 * tau[i-1];
        if (tau[i] > 1000) {
            break;
        }
        //qDebug() << tau[i];
    }

    int H = L;
    int count = L + 1;
    while (count < bits.size()) {
        H++;
        count += H + 1;
    }

    int Lm = Xb;
    int Hm = Lm;
    int j = 1;
    int countm = Xb;
    while (countm < Nmid) {
        Hm++;
        Lm--;
        countm += 2*(Xb-j);
        j++;
    }

    double K[16];
    double** eps = new double* [bsize];
    double* eta = new double[bsize];
    int* start = new int[bsize];
    for (int i=0; i<bsize; i++) {
        eps[i] = new double [Nmid + 16];
        for (int j=0; j<Nmid+16; j++) {
            eps[i][j] = qrand() / (double)RAND_MAX;
        }
        rand();
    }
    //qDebug() << eps[100][100];
    for (int b=0; b<Nb; b++) {
        int by = Yb * (b / Nbx);
        int bx = Xb * (b % Nbx);

        for (int i=0; i<Xb; i++) {
            for (int j=0; j<Yb; j++) {
                double v;
                int cb, cr;
                switch(ch) {
                case channels::BLUE:
                    v = BLUE(container.pixel(bx+i, by+j));
                    break;
                case channels::RED:
                    v = RED(container.pixel(bx+i, by+j));
                    break;
                case channels::GREEN:
                    v = GREEN(container.pixel(bx+i, by+j));
                    break;
                case channels::Y:
                    RGBtoYCbCr(container.pixel(bx+i, by+j), v, cr, cb);
                    break;
                }
                mean += v;
                stddev += v*v;
            }
        }
        mean /= (Xb*Yb);
        stddev /= (Xb*Yb);
        stddev -= mean*mean;
        stddev = sqrt(stddev);
        cnorm = 1024 / (Xb*stddev);
        for (int i=0; i<Xb; i++) {
            for (int j=0; j<Yb; j++) {
                int cr, cb;
                switch(ch) {
                case channels::BLUE:
                    A[i*Yb + j] = (BLUE(container.pixel(bx+i, by+j)) - mean) * cnorm;
                    break;
                case channels::RED:
                    A[i*Yb + j] = (RED(container.pixel(bx+i, by+j)) - mean) * cnorm;
                    break;
                case channels::GREEN:
                    A[i*Yb + j] = (GREEN(container.pixel(bx+i, by+j)) - mean) * cnorm;
                    break;
                case channels::Y:
                    RGBtoYCbCr(container.pixel(bx+i, by+j), A[i*Yb + j], cr, cb);
                    A[i*Yb + j] = (A[i*Yb + j] - mean) * cnorm;
                    break;
                }
            }
        }
        Dct.fdct2(A, B, Xb, Yb);
        count = 0;
        countm = 0;

        qDebug() << "mode " << mode;

        //if (mode & 2) {
            for (int i1=0; i1<Xb; i1++) {
                for (int i2=0; i2<Yb; i2++) {
                    if (Lm <= i1 + i2 + 1 && i1 + i2 + 1 <= Hm) {
                        mid[countm] = B[i1 * Yb + i2];

                        countm++;
                        if (countm >= Nmid) break;
                    }
                }
                if (countm >= Nmid) break;
            }
            int cc = Nmid * 8 / bits.size();
            int cn = bits.size() / 8;
            double cstep = 1.98 / double(cc);
            int index = 0;
            for (int i=0; i<cn; i++) {
                int fi_vals[8];
                for (int k=0; k<8; k++) {
                    double res = 0;
                    double x = -0.99;
                    //double x = -1;
                    for (int j=0; j<cc; j++) {
                        res += cstep * fi(Lc[k], x) * mid[i*cc + j] * (1.0 / sqrt(1 - x*x));
                        x = x + cstep;
                    }
                    //qDebug() << res;
                    if (res <= this->a) {
                        data[i*8 + k]--;
                    }
                    else {
                        data[i*8 + k]++;
                    }
                }
            }

            }
        //}
    //}

    for (int i=0; i<length; i++) {
        //qDebug() << data[i];
        if (data[i] > 0) {
            bits[i] = 1;
        }
        else {
            bits[i] = 0;
        }
    }
    watermark = bitToByte(bits);

    delete [] A;
    delete [] B;
    delete [] data;
    delete [] mid;
    for (int i=0; i<bsize; i++) {
        delete [] eps[i];
    }
    delete [] eps;
    delete [] eta;
}

void CAlgPolynom::Restore(QImage& container, QVector<double>& watermark, QByteArray& key)
{

    qDebug() << "polynom";
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    //qDebug() << keyData.seed;
    int length = keyData.length;
    //cout << keyData.length;
    QBitArray bits;
    bits.resize(length);
    int bsize = bits.size();
    //double gamma = 1;
    int k = 0;
    int l = 0;
    int X = container.width();
    int Y = container.height();
    int Nbx = 2;
    int Nby = 2;
    int Nb = Nbx*Nby;
    int Xb = X / Nbx;
    int Yb = Y / Nby;
    int Nmid = Xb*Yb*0.3; //4080;
    //int L = 20;
    double* A = new double [Xb*Yb];
    double* B = new double [Xb*Yb];
    QVector<double> data(length);
    //double* data = new double[length];
    for (int i=0; i<length; i++) {
        data[i] = 0;
    }
    double cnorm;
    double y, cb, cr;
    double mean = 0;
    double stddev = 0;
    DCT Dct;
    double* mid = new double [Nmid];
    //double a = 0.3;
    double a2 = (1 + a) / (1 - a);
    double tau[3000];
    qDebug() << "RESTORE******************";
    tau[0] = 1;
    int tau_max = 1;
    for (int i=1; i<3000; i++) {
        tau_max++;
        tau[i] = a2 * tau[i-1];
        if (tau[i] > 1000) {
            break;
        }
        //qDebug() << tau[i];
    }

    int H = L;
    int count = L + 1;
    while (count < bits.size()) {
        H++;
        count += H + 1;
    }

    int Lm = Xb;
    int Hm = Lm;
    int j = 1;
    int countm = Xb;
    while (countm < Nmid) {
        Hm++;
        Lm--;
        countm += 2*(Xb-j);
        j++;
    }

    double K[16];
    double** eps = new double* [bsize];
    double* eta = new double[bsize];
    int* start = new int[bsize];
    for (int i=0; i<bsize; i++) {
        eps[i] = new double [Nmid + 16];
        for (int j=0; j<Nmid+16; j++) {
            eps[i][j] = qrand() / (double)RAND_MAX;
        }
        rand();
    }
    //qDebug() << eps[100][100];
    for (int b=0; b<Nb; b++) {
        int by = Yb * (b / Nbx);
        int bx = Xb * (b % Nbx);

        for (int i=0; i<Xb; i++) {
            for (int j=0; j<Yb; j++) {
                double v;
                int cb, cr;
                switch(ch) {
                case channels::BLUE:
                    v = BLUE(container.pixel(bx+i, by+j));
                    break;
                case channels::RED:
                    v = RED(container.pixel(bx+i, by+j));
                    break;
                case channels::GREEN:
                    v = GREEN(container.pixel(bx+i, by+j));
                    break;
                case channels::Y:
                    RGBtoYCbCr(container.pixel(bx+i, by+j), v, cr, cb);
                    break;
                }
                mean += v;
                stddev += v*v;
            }
        }
        mean /= (Xb*Yb);
        stddev /= (Xb*Yb);
        stddev -= mean*mean;
        stddev = sqrt(stddev);
        cnorm = 1024 / (Xb*stddev);
        for (int i=0; i<Xb; i++) {
            for (int j=0; j<Yb; j++) {
                int cr, cb;
                switch(ch) {
                case channels::BLUE:
                    A[i*Yb + j] = (BLUE(container.pixel(bx+i, by+j)) - mean) * cnorm;
                    break;
                case channels::RED:
                    A[i*Yb + j] = (RED(container.pixel(bx+i, by+j)) - mean) * cnorm;
                    break;
                case channels::GREEN:
                    A[i*Yb + j] = (GREEN(container.pixel(bx+i, by+j)) - mean) * cnorm;
                    break;
                case channels::Y:
                    RGBtoYCbCr(container.pixel(bx+i, by+j), A[i*Yb + j], cr, cb);
                    A[i*Yb + j] = (A[i*Yb + j] - mean) * cnorm;
                    break;
                }
            }
        }
        Dct.fdct2(A, B, Xb, Yb);
        count = 0;
        countm = 0;

        qDebug() << "mode " << mode;

        //if (mode & 2) {
            for (int i1=0; i1<Xb; i1++) {
                for (int i2=0; i2<Yb; i2++) {
                    if (Lm <= i1 + i2 + 1 && i1 + i2 + 1 <= Hm) {
                        mid[countm] = B[i1 * Yb + i2];

                        countm++;
                        if (countm >= Nmid) break;
                    }
                }
                if (countm >= Nmid) break;
            }
            int cc = Nmid * 8 / bits.size();
            int cn = bits.size() / 8;
            double cstep = 1.98 / double(cc);
            int index = 0;
            double* res = new double[bits.size()];
            double res_min = 100, res_max = -100;
            for (int i=0; i<cn; i++) {
                int fi_vals[8];
                for (int k=0; k<8; k++) {
                    res[i * 8 + k] = 0;
                    double x = -0.99;
                    //double x = -1;
                    for (int j=0; j<cc; j++) {
                        res[i * 8 + k] += cstep * fi(Lc[k], x) * mid[i*cc + j] * (1.0 / sqrt(1 - x*x));
                        x = x + cstep;
                    }
                    if (res_min > res[i * 8 + k]) {
                        res_min = res[i * 8 + k];
                    }
                    if (res_max < res[i * 8 + k]) {
                        res_max = res[i * 8 + k];
                    }
                    //qDebug() << res;
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

            delete [] res;

            }
        //}
    //}

    for (int i=0; i<length; i++) {
        data[i] /= Nb;
    }
    watermark = data;

    delete [] A;
    delete [] B;
    //delete [] data;
    delete [] mid;
    for (int i=0; i<bsize; i++) {
        delete [] eps[i];
    }
    delete [] eps;
    delete [] eta;
}


void CAlgPolynom::GenKey(QByteArray& data)
{
    keyStruct keyData;
    keyData.seed = QTime::currentTime().msec();
    keyData.length = data.size()*8;

    key = QByteArray((char*)&keyData, sizeof(keyStruct));
}

void CAlgPolynom::SetParams(QString& params)
{
    CParamHelper ph;
    ph.AddToMap(&a, ph.DOUBLE, "alpha");
    ph.AddToMap(&gamma, ph.DOUBLE, "gamma");
    ph.AddToMap(&mode, ph.INT, "mode");
    ph.AddToMap(&spectrum, ph.INT, "spectrum");
    ph.AddToMap(&L, ph.INT, "l");
    ph.AddToMap((void*)&ch, ph.CHANNEL, "channel");
    ph.SetParams(params);
}

