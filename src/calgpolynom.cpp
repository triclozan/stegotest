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
    keyStruct keyData = *(keyStruct*)(key.constData()); // preparing the key
    qsrand(keyData.seed);
    QBitArray bits = byteToBit(watermark);

    int X = container.width();  // total width
    int Y = container.height(); // total height
    int Nbx = 2;        // number of blocks in a row
    int Nby = 2;        // number of blocks in a column
    int Nb = Nbx*Nby;   // block number
    int Xb = X / Nbx;   // block width
    int Yb = Y / Nby;   // block height
    QScopedArrayPointer<double> A(new double [Xb*Yb]); // input matrix
    QScopedArrayPointer<double> B(new double [Xb*Yb]); // output matrix

    int Nmid = Xb * Yb * 0.3; // number of middle coefficients used in a block
    QScopedArrayPointer<double> encData(new double [Nmid]); // array with encoded WM to add to input matrix

    QScopedArrayPointer<QScopedArrayPointer<int> > Cb(new QScopedArrayPointer<int>[Yb]); // array to store Cb components
    QScopedArrayPointer<QScopedArrayPointer<int> > Cr(new QScopedArrayPointer<int>[Yb]); // array to store Cr components

    for (int j=0; j<Yb; j++) { // Store original color components
        Cb[j].reset(new int[Xb]);
        Cr[j].reset(new int[Xb]);
    }

    //------------------- Determine bound diagonals for embedding area
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
    //-------------------

    DCT Dct;
    double cnorm;
    double mean = 0;
    double stddev = 0;

    //------------------- Generate additive WM
    int cc = Nmid * 8 / bits.size();
    int cn = bits.size() / 8;
    double cstep = 1.98 / double(cc);

    for (int i = 0; i < cn; i++) {
        int k[8];
        for (int l=0; l<8; l++) {
            k[l] = bits[i*8 + l];
            bits[i*8 + l];
        }
        double x = -0.99;

        for (int j=0; j<cc; j++) {
            double res = 0;
            for (int l=0; l<8; l++) {
                res += k[l] * fi(Lc[l], x);
            }
            x = x + cstep;
            encData[i * cc + j] = res;
        }
    }
    //-------------------
    for (int b=0; b<Nb; b++) {  //iterate through blocks
        int by = Yb * (b / Nby); // vertical pixel offset of block
        int bx = Xb * (b % Nbx); // horizontal pixel offset of block

        //----------------------Calculate mean and st. deviation of values in block
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
        //----------------------

        //-----------------------Form a matrix of normalized block values
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
        //----------------------

        Dct.fdct2(A.data(), B.data(), Xb, Yb);

        countm = 0;

        //-----------------Adding the WM
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

        //-----------------If we are to show spectrum instead of producing a watermarked image
        if (spectrum) {
            //-----------------Calculate the maximum value in the logarithm scale
            double valMax = 0;
            for (int i1=0; i1<Xb; i1++) {
                for (int i2=0; i2<Yb; i2++) {
                    double v = log(1 + B[i1*Yb + i2]);
                    if (v > valMax) {
                        valMax = v;
                    }
                }
            }

            //-----------------Turn spectrum into an image
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
        //-----------------If we are to produce a watermarked image
        else {
            Dct.idct2(B.data(), A.data(), Xb, Yb);

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
}

void CAlgPolynom::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData()); // preparing the key
    qsrand(keyData.seed);
    int length = keyData.length;

    QBitArray bits;
    bits.resize(length);

    int X = container.width();
    int Y = container.height();
    int Nbx = 2;
    int Nby = 2;
    int Nb = Nbx*Nby;
    int Xb = X / Nbx;
    int Yb = Y / Nby;
    int Nmid = Xb*Yb*0.3;

    QScopedArrayPointer<double> A(new double [Xb*Yb]); // input matrix
    QScopedArrayPointer<double> B(new double [Xb*Yb]); // output matrix
    QScopedArrayPointer<double> data(new double [length]); // output matrix
    QScopedArrayPointer<double> mid(new double [Nmid]); // output matrix

    for (int i=0; i<length; i++) {
        data[i] = 0;
    }

    double cnorm;
    double mean = 0;
    double stddev = 0;
    DCT Dct;

    //------------------- Determine bound diagonals for embedding area
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
    //-------------------

    for (int b=0; b<Nb; b++) {  //iterate through blocks
        int by = Yb * (b / Nby); // vertical pixel offset of block
        int bx = Xb * (b % Nbx); // horizontal pixel offset of block

        //----------------------Calculate mean and st. deviation of values in block
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
        //----------------------


        //-----------------------Form a matrix of normalized block values
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
        //----------------------

        Dct.fdct2(A, B, Xb, Yb);
        countm = 0;

        //---------------Linearizing changed part of matrix to an array
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

        //---------------Extracting WM values
        int cc = Nmid * 8 / bits.size();
        int cn = bits.size() / 8;
        double cstep = 1.98 / double(cc);
        for (int i=0; i<cn; i++) {
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

    //---------------Averaging WM values by blocks and taking a decision with threshold 0
    for (int i=0; i<length; i++) {
        if (data[i] > 0) {
            bits[i] = 1;
        }
        else {
            bits[i] = 0;
        }
    }
    //---------------
    watermark = bitToByte(bits);
}

void CAlgPolynom::Restore(QImage& container, QVector<double>& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData()); // preparing the key
    qsrand(keyData.seed);
    int length = keyData.length;

    QBitArray bits;
    bits.resize(length);

    int bsize = bits.size();

    int X = container.width();
    int Y = container.height();
    int Nbx = 2;
    int Nby = 2;
    int Nb = Nbx*Nby;
    int Xb = X / Nbx;
    int Yb = Y / Nby;
    int Nmid = Xb*Yb*0.3;

    QScopedArrayPointer<double> A(new double [Xb*Yb]); // input matrix
    QScopedArrayPointer<double> B(new double [Xb*Yb]); // output matrix
    QScopedArrayPointer<double> data(new double [length]); // output matrix
    QScopedArrayPointer<double> mid(new double [Nmid]); // output matrix

    for (int i=0; i<length; i++) {
        data[i] = 0;
    }

    double cnorm;
    double mean = 0;
    double stddev = 0;
    DCT Dct;

    //------------------- Determine bound diagonals for embedding area
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
    //-------------------

    for (int b=0; b<Nb; b++) {  //iterate through blocks
        int by = Yb * (b / Nby); // vertical pixel offset of block
        int bx = Xb * (b % Nbx); // horizontal pixel offset of block

        //----------------------Calculate mean and st. deviation of values in block
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
        //----------------------

        //-----------------------Form a matrix of normalized block values
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
        //----------------------

        Dct.fdct2(A, B, Xb, Yb);
        countm = 0;
        count = 0;

        //---------------Linearizing changed part of matrix to an array
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

        //---------------Extracting WM values
        int cc = Nmid * 8 / bits.size();
        int cn = bits.size() / 8;
        double cstep = 1.98 / double(cc);
        QScopedArrayPointer<double> res(new double [bits.size()]); // input matrix
        double res_min = 100, res_max = -100;

        for (int i=0; i<cn; i++) {
            for (int k=0; k<8; k++) {
                res[i * 8 + k] = 0;
                double x = -0.99;
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

    for (int i=0; i<length; i++) {
        data[i] /= Nb;
    }
    watermark = data;
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

