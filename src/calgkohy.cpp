#include "calgkohy.h"
#include "dct.h"
#include <QTime>
#include <math.h>
#include "ctransform.h"

void CAlgKohY::Hide(QImage& container, QByteArray& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    int tresh = *(int*)(params.constData());
    QBitArray bits = byteToBit(watermark);
    int k = 0;
    int l = 0;
    int m = 0;
    double A[8][8];
    int Cr[8][8];
    int Cb[8][8];
    double B[8][8];
    double p1, p2;
    double C1, C2;
    //qDebug() << tresh;
    //int tresh = 100;
    int x1 = 1, x2 = 2, y1 = 2, y2 = 1;
    double y, cb, cr;


    double **val = new double* [container.height()];
    for (int i=0; i<container.height(); i++) {
        val[i] = new double[container.width()];
    }
    for (int i=0; i<container.width(); i++) {
        for (int j=0; j<container.height(); j++) {
            val[i][j] = container.pixel(i, j) & 0xFF;
        }
    }

    DCT Dct;
    for (int i=0; i<bits.size(); i++) {
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                /*QColor color = QColor(container.pixel(k+i1, l+i2));
                Cb[i1][i2] = -0.169*color.red() - 0.331*color.green() + 0.499*color.blue();
                Cr[i1][i2] = 0.499*color.red() - 0.418*color.green() - 0.0813*color.blue();
                A[i1][i2] = 0.299*color.red() + 0.587*color.green() + 0.114*color.blue();*/
                RGBtoYCbCr(container.pixel(k+i1, l+i2), A[i1][i2], Cr[i1][i2], Cb[i1][i2]);
            }
        }
        Dct.fdct(A, B);

        p1 = B[x1][y1];
        p2 = B[x2][y2];
        if (bits[i] == 0) {
            //cout << p1 << " " << p2 << " ";
            if (fabs(p1) - fabs(p2) < tresh) {
                double a = (tresh - fabs(p1) + fabs(p2));
                (p1 > 0) ? (p1 += a) : (p1 -= a);
            }
        }
        else {
            if (fabs(p1) - fabs(p2) > -tresh) {
                double a = (tresh + fabs(p1) - fabs(p2));
                (p2 > 0) ? (p2 += a) : (p2 -= a);
            }
        }

        B[x1][y1] = p1;
        B[x2][y2] = p2;
        Dct.idct(B, A);

        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                /*QColor color;
                double Y = A[i1][i2];
                double R = Y + Cr[i1][i2] + (Cr[i1][i2] >> 2) + (Cr[i1][i2] >> 3) + (Cr[i1][i2] >> 5);
                double G = Y - ((Cb[i1][i2] >> 2) + (Cb[i1][i2] >> 4) + (Cb[i1][i2] >> 5)) - ((Cr[i1][i2] >> 1) + (Cr[i1][i2] >> 3) + (Cr[i1][i2] >> 4) + (Cr[i1][i2] >> 5));
                double B = Y + Cb[i1][i2] + (Cb[i1][i2] >> 1) + (Cb[i1][i2] >> 2) + (Cb[i1][i2] >> 6);
                color.setRed(PIXELRANGE(ROUND(R)));
                color.setGreen(PIXELRANGE(ROUND(G)));
                color.setBlue(PIXELRANGE(ROUND(B)));

                QRgb val = color.rgb();
                container.setPixel(k+i1, l+i2, val);*/
                container.setPixel(k+i1, l+i2, YCbCrtoRGB(A[i1][i2], Cr[i1][i2], Cb[i1][i2]));
            }
        }

        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                val[k+i1][l+i2] = A[i1][i2];
            }
        }

        k += 8;
        if (k >= container.width()) {
            k = 0;
            l += 8;
        }
    }
    CTransform t;
    //t.LinearCorrection(container, val, 0);
    for (int i=0; i<container.height(); i++) {
        delete [] val[i];
    }
    delete [] val;
    //container.save("F:\\koh.bmp");
}

void CAlgKohY::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    int length = keyData.length;
    //cout << keyData.length;
    QBitArray bits;
    bits.resize(length);
    int x1 = 1, x2 = 2, y1 = 2, y2 = 1;
    int k = 0;
    int l = 0;
    int tresh = *(int*)(params.constData());
    //qDebug() << tresh;
    double A[8][8];
    double B[8][8];
    int Cr[8][8];
    int Cb[8][8];
    double p1, p2;
    double C1, C2;

    DCT Dct;
    for (int i=0; i<bits.size(); i++) {
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                /*QColor color = QColor(container.pixel(k+i1, l+i2));
                Cb[i1][i2] = -0.169*color.red() - 0.331*color.green() + 0.499*color.blue();
                Cr[i1][i2] = 0.499*color.red() - 0.418*color.green() - 0.0813*color.blue();
                A[i1][i2] = 0.299*color.red() + 0.587*color.green() + 0.114*color.blue();*/
                RGBtoYCbCr(container.pixel(k+i1, l+i2), A[i1][i2], Cr[i1][i2], Cb[i1][i2]);
            }
        }
        Dct.fdct(A, B);
        p1 = B[x1][y1];
        p2 = B[x2][y2];
        //cout << fabs(p1) - fabs(p2) << "\n";
        if (fabs(p1) - fabs(p2) > 0) {
            bits[i] = 0;
        }
        else {
            bits[i] = 1;
        }

        k += 8;
        if (k >= container.width()) {
            k = 0;
            l += 8;
        }
    }
    watermark = bitToByte(bits);
}

void CAlgKohY::GenKey(QByteArray& data)
{
    keyStruct keyData;
    keyData.seed = QTime::currentTime().msec();
    keyData.length = data.size()*8;

    key = QByteArray((char*)&keyData, sizeof(keyStruct));
}
