#include "calgbengam.h"
#include "dct.h"
#include <QTime>
#include <math.h>

void CAlgBengam::Hide(QImage& container, QByteArray& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    int tresh = *(int*)(params.constData());
    int PH = 10;
    int PL = 2600;
    QBitArray bits = byteToBit(watermark);
    int k = 0;
    int l = 0;
    int m = 0;
    double A[8][8];
    double B[8][8];
    double p1, p2, p3;
    double C1, C2;
    //qDebug() << tresh;
    //int tresh = 100;
    int x1 = 2, x2 = 4, x3 = 6, y1 = 6, y2 = 4, y3 = 2;
    double y, cb, cr;
    DCT Dct;
    for (int i=0; i<bits.size(); i++) {
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                A[i1][i2] = container.pixel(k+i1, l+i2) & 0xFF;
            }
        }

        Dct.fdct(A, B);

        double low = 0, high = 0;
        for (int i=0; i<8-1; i++) {
            for (int j=0; j<8-1-i; j++) {
                low += fabs(B[i][j]);
            }
        }
        //qDebug() << "L:" << low;
        if (low > PL) {
            k += 8;
            if (k >= container.width()) {
                k = 0;
                l += 8;
            }
            i--;
            continue;
        }

        for (int i=0; i<8-2; i++) {
            for (int j=0; j<8-2-i; j++) {
                high += fabs(B[8-1-i][8-1-j]);
            }
        }
        //qDebug() << "H:" << high;
        if (high < PH) {
            k += 8;
            if (k >= container.width()) {
                k = 0;
                l += 8;
            }
            i--;
            continue;
        }

        p1 = B[x1][y1];
        p2 = B[x2][y2];
        p3 = B[x3][y3];
        if (bits[i] == 0) {
            if (fabs(p3) > fabs(p2) || fabs(p3) > fabs(p1)) {
                double v_min = (p1 > p2) ? p2 : p1;
                p3 = 0;
                (p1 > 0) ? (p1 += tresh) : (p1 -= tresh);
                (p2 > 0) ? (p2 += tresh) : (p2 -= tresh);
            }
                /*if (p3 > p2 || p3 > p1) {
                double v_min = (p1 > p2) ? p2 : p1;
                //double a = (tresh + v_min - p3);

                p3 = v_min - tresh / 2.0;
                (p1 > p2) ? (p2 += tresh / 2.0) : (p1 += tresh / 2.0);
            }*/
        }
        else {
            if (fabs(p3) < fabs(p2) || fabs(p3) < fabs(p1)) {
                double v_max = (p1 > p2) ? p1 : p2;
                (p3 > 0) ? (p3 += tresh) : (p3 -= tresh);
                p1 = p2 = 0;
            }
            /*if (p3 < p2 || p3 < p1) {
                double v_max = (p1 > p2) ? p1 : p2;
                //double a = (tresh + v_min - p3);
                p3 = v_max + tresh / 2.0;
                (p1 > p2) ? (p1 -= tresh / 2.0) : (p2 -= tresh / 2.0);
            }*/
        }
        B[x1][y1] = p1;
        B[x2][y2] = p2;
        B[x3][y3] = p3;
        Dct.idct(B, A);
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                QColor color = QColor(container.pixel(k+i1, l+i2));
                color.setBlue(PIXELRANGE(ROUND(A[i1][i2])));
                if (i < 10) {
                    //cout << PIXELRANGE(ROUND(A[i1][i2])) << "\n";
                }
                QRgb val = color.rgb();
                container.setPixel(k+i1, l+i2, val);
            }
        }
        k += 8;
        if (k >= container.width()) {
            k = 0;
            l += 8;
        }
    }
    //qDebug() << l * container.width() + k;
    container.save("F:\\koh.bmp");
}

void CAlgBengam::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    int length = keyData.length;
    //cout << keyData.length;
    QBitArray bits;
    bits.resize(length);
    int x1 = 2, x2 = 4, x3 = 6, y1 = 6, y2 = 4, y3 = 2;
    int k = 0;
    int l = 0;
    int PH = 10;
    int PL = 2600;
    int tresh = *(int*)(params.constData());
    //qDebug() << tresh;
    double A[8][8];
    double B[8][8];
    double p1, p2, p3;
    double C1, C2;
    DCT Dct;
    for (int i=0; i<bits.size(); i++) {
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                A[i1][i2] = container.pixel(k+i1, l+i2) & 0xFF;
            }
        }
        Dct.fdct(A, B);
        double low = 0, high = 0;
        for (int i=0; i<8-1; i++) {
            for (int j=0; j<8-1-i; j++) {
                low += fabs(B[i][j]);
            }
        }
        //qDebug() << "L:" << low;
        if (low > PL) {
            k += 8;
            if (k >= container.width()) {
                k = 0;
                l += 8;
            }
            i--;
            continue;
        }

        for (int i=0; i<8-2; i++) {
            for (int j=0; j<8-2-i; j++) {
                high += fabs(B[8-1-i][8-1-j]);
            }
        }
        //qDebug() << "H:" << high;
        if (high < PH) {
            k += 8;
            if (k >= container.width()) {
                k = 0;
                l += 8;
            }
            i--;
            continue;
        }

        p1 = B[x1][y1];
        p2 = B[x2][y2];
        p3 = B[x3][y3];

        //cout << fabs(p1) - fabs(p2) << "\n";
        /*double v_min = (p1 > p2) ? p2 : p1;
        double v_max = (fabs(p1) > fabs(p2)) ? fabs(p1) : fabs(p2);
        if (p3 < v_min) {
            bits[i] = 0;
        }
        else {
            bits[i] = 1;
        }*/
        double v_min = (fabs(p1) > fabs(p2)) ? fabs(p2) : fabs(p1);
        double v_max = (fabs(p1) > fabs(p2)) ? fabs(p1) : fabs(p2);
        if (fabs(p3) < v_min) {
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
    //qDebug() << l * container.width() + k;
    watermark = bitToByte(bits);
}


void CAlgBengam::GenKey(QByteArray& data)
{
    keyStruct keyData;
    keyData.seed = QTime::currentTime().msec();
    keyData.length = data.size()*8;

    key = QByteArray((char*)&keyData, sizeof(keyStruct));
}
