#include "calgkoh.h"
#include "dct.h"
#include <QTime>
#include <math.h>
#include "ctransform.h"
#include "util.h"

CAlgKoh::CAlgKoh() : tresh(10), ch(3) {}

void CAlgKoh::Hide(QImage& container, QByteArray& watermark, QByteArray& key)
{
    //qDebug() << tresh << ch;
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    //int tresh = *(int*)(params.constData());
    //qDebug() << tresh;
    QBitArray bits = byteToBit(watermark);
    int k = 0;
    int l = 0;
    int m = 0;
    int Cr[8][8];
    int Cb[8][8];
    double A[8][8];
    double B[8][8];
    double p1, p2;
    double C1, C2;
    //qDebug() << tresh;
    //int tresh = 100;
    int x1 = 2, x2 = 3, y1 = 3, y2 = 2;
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
 /*   for (int i=0; i<bits.size(); i++) {
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                (i1 == 0) ? (C1 = 1 / sqrt(2)) : (C1 = 1);
                (i2 == 0) ? (C2 = 1 / sqrt(2)) : (C2 = 1);
                double sum = 0;
                for (int i3=0; i3<8; i3++) {
                    for (int i4=0; i4<8; i4++) {
                        int a = QColor(container.pixel(k+i3, l+i4)).blue();
                        sum += (QColor(container.pixel(k+i3, l+i4)).blue()-128) * cos(M_PI * i1 * (2 * i3 + 1) / 16.0) * cos(M_PI * i2 * (2 * i4 + 1) / 16.0);
                    }
                }
                A[i1][i2] = C1*C2*sum/4.0;
            }
        }
        p1 = A[x1][y1];
        p2 = A[x2][y2];
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
        A[x1][y1] = p1;
        A[x2][y2] = p2;
        double min_b = 10000;
        double max_b = -10000;
        //cout << A[3][5] << " " << A[5][3] << " ";
        //cout << fabs(p1) - fabs(p2) << "\n";
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                double sum = 0;
                for (int i3=0; i3<8; i3++) {
                    for (int i4=0; i4<8; i4++) {
                        (i3 == 0) ? (C1 = 1 / sqrt(2)) : (C1 = 1);
                        (i4 == 0) ? (C2 = 1 / sqrt(2)) : (C2 = 1);
                        sum += C1*C2*A[i3][i4] * cos(M_PI * i3 * (2 * i1 + 1) / 16.0) * cos(M_PI * i4 * (2 * i2 + 1) / 16.0);
                    }
                }
                sum = sum / 4 + 128;
                if (sum < min_b) {
                    min_b = sum;
                }
                if (sum > max_b) {
                    max_b = sum;
                }
                int old_sum = sum;
                B[i1][i2] = sum;
                if (sum < 0) {sum = 0;}
                if (sum > 255) {sum = 255;}
                //cout << sum << "\n";

                QColor color = QColor(container.pixel(k+i1, l+i2));
                color.setBlue(uchar(sum));
                cout << old_sum << "\n";
                QRgb val = color.rgb();
                container.setPixel(k+i1, l+i2, val);
                //QColor(container.pixel(k+i1, l+i2)).setBlue(sum);
            }
        }
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                (i1 == 0) ? (C1 = 1 / sqrt(2)) : (C1 = 1);
                (i2 == 0) ? (C2 = 1 / sqrt(2)) : (C2 = 1);
                double sum = 0;
                for (int i3=0; i3<8; i3++) {
                    for (int i4=0; i4<8; i4++) {
                        int a = QColor(container.pixel(k+i3, l+i4)).blue();
                        //cout << B[i3][i4] << "\n";
                        sum += (QColor(container.pixel(k+i3, l+i4)).blue()-128) * cos(M_PI * i1 * (2 * i3 + 1) / 16.0) * cos(M_PI * i2 * (2 * i4 + 1) / 16.0);
                    }
                }
                A[i1][i2] = C1*C2*sum/4.0;
            }
        }
        p1 = A[x1][y1];
        p2 = A[x2][y2];
        //cout << fabs(p1) - fabs(p2) << "\n";
        k += 8;
        if (k >= container.width()) {
            k = 0;
            l += 8;
        }
    }*/
    DCT Dct;
    for (int i=0; i<bits.size(); i++) {
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                switch(ch) {
                case channels::BLUE:
                    A[i1][i2] = BLUE(container.pixel(k+i1, l+i2));
                    break;
                case channels::RED:
                    A[i1][i2] = RED(container.pixel(k+i1, l+i2));
                    break;
                case channels::GREEN:
                    A[i1][i2] = GREEN(container.pixel(k+i1, l+i2));
                    break;
                case channels::Y:
                    RGBtoYCbCr(container.pixel(k+i1, l+i2), A[i1][i2], Cr[i1][i2], Cb[i1][i2]);
                    break;
                }
            }
        }
        Dct.fdct(A, B);
        /*if (i == 1) {
            for (int i=0; i<8; i++) {
                QString str;
                for (int j=0; j<8; j++) {
                    str += QString::number(B[i][j]) + " ";
                }
                qDebug() << str;
            }
        }*/
        p1 = B[x1][y1];
        p2 = B[x2][y2];
        if (bits[i] == 0) {
            //cout << p1 << " " << p2 << " ";
            if (fabs(p1) - fabs(p2) < tresh) {
                double a = (tresh - fabs(p1) + fabs(p2));
                /*if (fabs(p2) < a/2) {
                    p2 = 0;
                    a = (tresh - fabs(p1));
                }
                else {
                    (p2 > 0) ? (p2 -= a/2) : (p2 += a/2);
                    a /= 2;
                }*/
                (p1 > 0) ? (p1 += a) : (p1 -= a);
            }
        }
        else {
            if (fabs(p1) - fabs(p2) > -tresh) {
                double a = (tresh + fabs(p1) - fabs(p2));
                /*if (fabs(p1) < a/2) {
                    p1 = 0;
                    a = (tresh - fabs(p2));
                }
                else {
                    (p1 > 0) ? (p1 -= a/2) : (p1 += a/2);
                    a /= 2;
                }*/
                (p2 > 0) ? (p2 += a) : (p2 -= a);
            }
        }
        /*if (bits[i] == 1) {
            if (fabs(p1) < 1.5*tresh) {
                (p1 > 0) ? (p1 += 1.5*tresh - fabs(p1)) : (p1 -= 1.5*tresh - fabs(p1));
            }
        }
        else {
            if (fabs(p1) > 0.5*tresh) {
                (p1 > 0) ? (p1 -= fabs(p1) - 0.5*tresh) : (p1 += fabs(p1) - 0.5*tresh);
            }
        }*/
        /*double diff = fabs(p1) - fabs(p2);
        double abs_diff = (fabs(diff) + tresh) / 2.0;
        cout << fabs(p1) - fabs(p2) << "\n";
        if (bits[i] == 0) {
          if (diff < tresh) {
            (p1 > 0.0) ? (p1 += abs_diff) : (p1 -= abs_diff);
            (p2 > 0.0) ? (p2 -= abs_diff) : (p2 += abs_diff);
          }
        }
        else {
          if (diff > -tresh) {
            (p1 > 0.0) ? (p1 -= abs_diff) : (p1 += abs_diff);
            (p2 > 0.0) ? (p2 += abs_diff) : (p2 -= abs_diff);
          }
        }
        cout << fabs(p1) - fabs(p2) << "\n";*/
        B[x1][y1] = p1;
        B[x2][y2] = p2;
        Dct.idct(B, A);

        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                switch(ch) {
                case channels::BLUE:
                    container.setPixel(k+i1, l+i2, SET_BLUE(container.pixel(k+i1, l+i2), PIXELRANGE(ROUND(A[i1][i2]))));
                    break;
                case channels::RED:
                    container.setPixel(k+i1, l+i2, SET_RED(container.pixel(k+i1, l+i2), PIXELRANGE(ROUND(A[i1][i2]))));
                    break;
                case channels::GREEN:
                    container.setPixel(k+i1, l+i2, SET_GREEN(container.pixel(k+i1, l+i2), PIXELRANGE(ROUND(A[i1][i2]))));
                    break;
                case channels::Y:
                    container.setPixel(k+i1, l+i2, YCbCrtoRGB(A[i1][i2], Cr[i1][i2], Cb[i1][i2]));
                    break;
                }
                /*QColor color = QColor(container.pixel(k+i1, l+i2));
                color.setBlue(PIXELRANGE(ROUND(A[i1][i2])));
                QRgb val = color.rgb();
                container.setPixel(k+i1, l+i2, val);*/
            }
        }
        /*if (i == 1) {
            for (int i1=0; i1<8; i1++) {
                for (int i2=0; i2<8; i2++) {
                    A[i1][i2] = container.pixel(k+i1, l+i2) & 0xFF;
                }
            }
            qDebug() << "";
            Dct.fdct(A, B);
            for (int i=0; i<8; i++) {
                QString str;
                for (int j=0; j<8; j++) {
                    str += QString::number(B[i][j]) + " ";
                }
                qDebug() << str;
            }
        }*/

        /*for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                val[k+i1][l+i2] = A[i1][i2];
            }
        }*/

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

void CAlgKoh::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    int length = keyData.length;
    //cout << keyData.length;
    QBitArray bits;
    bits.resize(length);
    int x1 = 2, x2 = 3, y1 = 3, y2 = 2;
    int k = 0;
    int l = 0;  
    //int tresh = *(int*)(params.constData());
    //qDebug() << tresh;
    double A[8][8];
    double B[8][8];
    double p1, p2;
    double C1, C2;
    //cout << bits.size();
    //return;
    /*for (int i=0; i<bits.size(); i++) {
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                (i1 == 0) ? (C1 = 1 / sqrt(2)) : (C1 = 1);
                (i2 == 0) ? (C2 = 1 / sqrt(2)) : (C2 = 1);
                double sum = 0;
                for (int i3=0; i3<8; i3++) {
                    for (int i4=0; i4<8; i4++) {
                        int a = QColor(container.pixel(k+i3, l+i4)).blue();
                        sum += (QColor(container.pixel(k+i3, l+i4)).blue()-128) * cos(M_PI * i1 * (2 * i3 + 1) / 16.0) * cos(M_PI * i2 * (2 * i4 + 1) / 16.0);
                    }
                }
                A[i1][i2] = C1*C2*sum/4.0;
            }
        }
        p1 = A[x1][y1];
        p2 = A[x2][y2];
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
    }*/
    DCT Dct;

    int cr, cb;
    for (int i=0; i<bits.size(); i++) {
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                switch(ch) {
                case channels::BLUE:
                    A[i1][i2] = BLUE(container.pixel(k+i1, l+i2));
                    break;
                case channels::RED:
                    A[i1][i2] = RED(container.pixel(k+i1, l+i2));
                    break;
                case channels::GREEN:
                    A[i1][i2] = GREEN(container.pixel(k+i1, l+i2));
                    break;
                case channels::Y:
                    RGBtoYCbCr(container.pixel(k+i1, l+i2), A[i1][i2], cr, cb);
                    break;
                }
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
        /*if (fabs(p1) < tresh) {
            bits[i] = 0;
        }
        else {
            bits[i] = 1;
        }*/
        k += 8;
        if (k >= container.width()) {
            k = 0;
            l += 8;
        }
    }
    watermark = bitToByte(bits);
}




void CAlgKoh::GenKey(QByteArray& data)
{
    keyStruct keyData;
    keyData.seed = QTime::currentTime().msec();
    keyData.length = data.size()*8;

    key = QByteArray((char*)&keyData, sizeof(keyStruct));
}

void CAlgKoh::SetParams(QString& params)
{
    CParamHelper ph;
    //qDebug() << "AAA";
    ph.AddToMap((void*)&tresh, ph.DOUBLE, "threshold");
    ph.AddToMap((void*)&ch, ph.CHANNEL, "channel");
    ph.SetParams(params);
}
