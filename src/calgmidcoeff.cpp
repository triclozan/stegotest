#include "calgmidcoeff.h"
#include "dct.h"
#include <QTime>
#include <math.h>
#include "util.h"

CAlgMidCoeff::CAlgMidCoeff() : intMethod("rectangle") {
    a = 0.35;
    gamma = 1;
    mode = 3;
    ch = 3;
    spectrum = 0;
}

void CAlgMidCoeff::GenerateWM(double* encData, int Nmid, QBitArray bits)
{

}

void CAlgMidCoeff::ExtractWM(double *data, double *mid, int Nmid, int size)
{

}

void CAlgMidCoeff::ExtractExtWM(double *data, double *mid, int Nmid, int size)
{

}

void CAlgMidCoeff::Hide(QImage& container, QByteArray& watermark, QByteArray& key)
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
    GenerateWM(encData.data(), Nmid, bits);
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

void CAlgMidCoeff::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
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
        //----------------------

        Dct.fdct2(A.data(), B.data(), Xb, Yb);
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
        ExtractWM(data.data(), mid.data(), Nmid, bits.size());
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

void CAlgMidCoeff::Restore(QImage& container, QVector<double>& watermark, QByteArray& key)
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
        //----------------------

        Dct.fdct2(A.data(), B.data(), Xb, Yb);
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
        ExtractExtWM(data.data(), mid.data(), Nmid, bits.size());
    }

    for (int i=0; i<length; i++) {
        data[i] /= Nb;
    }
    watermark = QVector<double>(length);
    for (int i=0; i<length; i++) {
        watermark[i] = data[i];
    }

}


void CAlgMidCoeff::GenKey(QByteArray& data)
{
    keyStruct keyData;
    keyData.seed = QTime::currentTime().msec();
    keyData.length = data.size()*8;

    key = QByteArray((char*)&keyData, sizeof(keyStruct));
}

void CAlgMidCoeff::SetParams(QString& params)
{
    CParamHelper ph;
    ph.AddToMap(&a, ph.DOUBLE, "alpha");
    ph.AddToMap(&gamma, ph.DOUBLE, "gamma");
    ph.AddToMap(&intMethod, ph.STRING, "intmethod");
    ph.AddToMap(&mode, ph.INT, "mode");
    ph.AddToMap(&spectrum, ph.INT, "spectrum");
    ph.AddToMap((void*)&ch, ph.CHANNEL, "channel");
    ph.SetParams(params);
}
