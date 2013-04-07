#include "calglangelaar.h"

#include "calgkoh.h"
#include "dct.h"
#include <QTime>
#include <math.h>
#include "ctransform.h"
#include "util.h"
#include <vector>
#include <algorithm>
using namespace std;

CAlgLangelaar::CAlgLangelaar() : tresh(100), ch(3) {}

unsigned int f(int ch, int delta, unsigned int u)
{
    double y;
    int cb, cr;
    switch(ch) {
    case channels::BLUE:
        return SET_BLUE(u, PIXELRANGE(BLUE(u) + delta));
        break;
    case channels::RED:
        return SET_RED(u, PIXELRANGE(RED(u) + delta));
        break;
    case channels::GREEN:
        return SET_GREEN(u, PIXELRANGE(GREEN(u) + delta));
        break;
    case channels::Y:
        RGBtoYCbCr(u, y, cr, cb);
        return YCbCrtoRGB(PIXELRANGE(y+delta), cr, cb);
        break;
    }
}

void CAlgLangelaar::Hide(QImage& container, QByteArray& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    QBitArray bits = byteToBit(watermark);
    int k = 0;
    int l = 0;
    int m = 0;
    int Cr[8][8];
    int Cb[8][8];
    //bool M[8][8];
    double y, cb, cr;
    //qDebug() << "size: " << bits.size();
    for (int i=0; i<bits.size(); i++) {
        vector<int> seq(64);
        for (int i=0; i<32; i++) {
            seq[i] = 0;
            seq[i+32] = 1;
        }
        random_shuffle(seq.begin(), seq.end()/*, p_myrandom*/);
        double s1 = 0, s0 = 0;
        double c1 = 0, c0 = 0;
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                //M[i1][i2] = rand() % 2;
                double y;
                int cr, cb;
                if (seq[i1 * 8 + i2]) {
                    switch(ch) {
                    case channels::BLUE:
                        s1 += BLUE(container.pixel(k+i1, l+i2));
                        break;
                    case channels::RED:
                        s1 += RED(container.pixel(k+i1, l+i2));
                        break;
                    case channels::GREEN:
                        s1 += GREEN(container.pixel(k+i1, l+i2));
                        break;
                    case channels::Y:
                        RGBtoYCbCr(container.pixel(k+i1, l+i2), y, cr, cb);
                        s1 += y;
                        break;
                    }
                    c1++;
                }
                else {
                    switch(ch) {
                    case channels::BLUE:
                        s0 += BLUE(container.pixel(k+i1, l+i2));
                        break;
                    case channels::RED:
                        s0 += RED(container.pixel(k+i1, l+i2));
                        break;
                    case channels::GREEN:
                        s0 += GREEN(container.pixel(k+i1, l+i2));
                        break;
                    case channels::Y:
                        RGBtoYCbCr(container.pixel(k+i1, l+i2), y, cr, cb);
                        s0 += y;
                        break;
                    }
                    c0++;
                }
            }
        }

        double diff = s1 - s0;
        //qDebug() << "NNN: " << s1 - s0;
        if (bits[i]) {
            if (diff < tresh) {
                int v = ceil((tresh - diff));
                int q = v / 64;
                int r = v % 64;
                int n = r;
                //qDebug() << "q: " << q << " v: " << v << " r: " << r;
                for (int i1=0; i1<8; i1++) {
                    for (int i2=0; i2<8; i2++) {
                        if (seq[i1 * 8 + i2]) {
                            if (n > 0) {
                                //qDebug() << "1!";
                                container.setPixel(k+i1, l+i2, f(ch, q+1, container.pixel(k+i1, l+i2)));
                            }
                            else {
                                container.setPixel(k+i1, l+i2, f(ch, q, container.pixel(k+i1, l+i2)));
                            }
                        }
                        else {
                            if (n > 0) {
                                //qDebug() << "0!";
                                container.setPixel(k+i1, l+i2, f(ch, -q-1, container.pixel(k+i1, l+i2)));
                            }
                            else {
                                container.setPixel(k+i1, l+i2, f(ch, -q, container.pixel(k+i1, l+i2)));
                            }
                        }
                        n--;
                    }
                }
            }
        }
        else {
            if (-diff < tresh) {
                int v = ceil((tresh + diff));
                int q = v / 64;
                int r = v % 64;
                int n = r;
                for (int i1=0; i1<8; i1++) {
                    for (int i2=0; i2<8; i2++) {
                        if (seq[i1 * 8 + i2]) {
                            if (n > 0) {
                                container.setPixel(k+i1, l+i2, f(ch, -q-1, container.pixel(k+i1, l+i2)));
                            }
                            else {
                                container.setPixel(k+i1, l+i2, f(ch, -q, container.pixel(k+i1, l+i2)));
                            }
                        }
                        else {
                            if (n > 0) {
                                container.setPixel(k+i1, l+i2, f(ch, q+1, container.pixel(k+i1, l+i2)));
                            }
                            else {
                                container.setPixel(k+i1, l+i2, f(ch, q, container.pixel(k+i1, l+i2)));
                            }
                        }
                        n--;
                    }
                }
            }
        }
        s1 = s0 = 0;
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                if (seq[i1 * 8 + i2]) {
                    s1 += BLUE(container.pixel(k+i1, l+i2));
                    c1++;
                }
                else {
                    s0 += BLUE(container.pixel(k+i1, l+i2));
                    c0++;
                }
            }
        }
        qDebug() << "WWW: " << fabs(s1-s0);
        k += 8;
        if (k >= container.width()) {
            k = 0;
            l += 8;
        }
    }
    //CTransform t;
    //t.LinearCorrection(container, val, 0);
    //container.save("F:\\koh.bmp");
}

void CAlgLangelaar::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    int length = keyData.length;
    QBitArray bits;
    bits.resize(length);
    int x1 = 2, x2 = 3, y1 = 3, y2 = 2;
    int k = 0;
    int l = 0;
    int cr, cb;
    //bool M[8][8];

    for (int i=0; i<bits.size(); i++) {
        vector<int> seq(64);
        for (int i=0; i<32; i++) {
            seq[i] = 0;
            seq[i+32] = 1;
        }
        random_shuffle(seq.begin(), seq.end()/*, p_myrandom*/);
        double s1 = 0, s0 = 0;
        for (int i1=0; i1<8; i1++) {
            for (int i2=0; i2<8; i2++) {
                double y;
                int cr, cb;
                if (seq[i1 * 8 + i2]) {
                    switch(ch) {
                    case channels::BLUE:
                        s1 += BLUE(container.pixel(k+i1, l+i2));
                        break;
                    case channels::RED:
                        s1 += RED(container.pixel(k+i1, l+i2));
                        break;
                    case channels::GREEN:
                        s1 += GREEN(container.pixel(k+i1, l+i2));
                        break;
                    case channels::Y:
                        RGBtoYCbCr(container.pixel(k+i1, l+i2), y, cr, cb);
                        s1 += y;
                        break;
                    }
                }
                else {
                    switch(ch) {
                    case channels::BLUE:
                        s0 += BLUE(container.pixel(k+i1, l+i2));
                        break;
                    case channels::RED:
                        s0 += RED(container.pixel(k+i1, l+i2));
                        break;
                    case channels::GREEN:
                        s0 += GREEN(container.pixel(k+i1, l+i2));
                        break;
                    case channels::Y:
                        RGBtoYCbCr(container.pixel(k+i1, l+i2), y, cr, cb);
                        s0 += y;
                        break;
                    }
                }
            }
        }

        if (s1 > s0) {
            bits[i] = 1;
        }
        else {
            bits[i] = 0;
        }
        k += 8;
        if (k >= container.width()) {
            k = 0;
            l += 8;
        }
    }
    watermark = bitToByte(bits);
}

void CAlgLangelaar::GenKey(QByteArray& data)
{
    keyStruct keyData;
    keyData.seed = QTime::currentTime().msec();
    keyData.length = data.size()*8;

    key = QByteArray((char*)&keyData, sizeof(keyStruct));
}

void CAlgLangelaar::SetParams(QString& params)
{
    CParamHelper ph;
    ph.AddToMap((void*)&tresh, ph.DOUBLE, "threshold");
    ph.AddToMap((void*)&ch, ph.CHANNEL, "channel");
    ph.SetParams(params);
}
