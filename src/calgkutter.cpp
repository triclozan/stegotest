#include "calgkutter.h"
#include <QDebug>
#include <iostream>
#include <QTime>
#include <vector>
//#include <iterator>
#include <algorithm>
#include "util.h"
#include "math.h"
using namespace std;

ptrdiff_t myrandom (ptrdiff_t i) { return qrand()%i;}
ptrdiff_t (*p_myrandom)(ptrdiff_t) = myrandom;

CAlgKutter::CAlgKutter() {
    v = 0.1;
    r = 16;
    len = 3;
    ch = channels::BLUE;
}

struct sPoint {
    int x;
    int y;
    sPoint(int px, int py): x(px), y(py) {}
};

void CAlgKutter::Hide(QImage& container, QByteArray& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData());
    //qDebug() << keyData.seed << "\n";
    qsrand(keyData.seed);
    qDebug() << "ch " << ch;
    //srand(keyData.seed);
    vector<int> seq(container.width()*container.height());
    for (int i=0; i<container.width()*container.height(); i++) {
        seq[i] = i;
    }
    random_shuffle(seq.begin(), seq.end()/*, p_myrandom*/);
    random_shuffle(seq.begin(), seq.end()/*, p_myrandom*/);
    random_shuffle(seq.begin(), seq.end()/*, p_myrandom*/);
    random_shuffle(seq.begin(), seq.end()/*, p_myrandom*/);
    random_shuffle(seq.begin(), seq.end()/*, p_myrandom*/);
    for (int i=0; i<container.width()*container.height(); i++) {
        if (seq[i] == 260000)
            qDebug() << "index " << i;
    }
    //qDebug() << "Wah!!!";
    //return;
    QBitArray bits = byteToBit(watermark);
    int rep = container.width()*container.height() / (bits.size()*r);
    int k = 0;
    int l = 0;
    int b, cb, cr;
    int m = 0;
    double db;
    for (int i=0; i<bits.size(); i++) {
        for (int t=0; t<rep; t++) {
            k = seq[m] % container.width();
            l = seq[m] / container.width();
            m++;
            //qDebug() << k << " " << l;
            if (k < len || l < len || k > container.width() - len - 1 || l > container.height() - len - 1) {
                t--;
                continue;
            }
            QColor value = container.pixel(k, l);
            double y;// = 0.29890 * value.red() + 0.58662 * value.green() + 0.11448 * value.blue();
            RGBtoYCbCr(container.pixel(k, l), y, cr, cb);
            switch(ch) {
            case channels::BLUE:
                b = (bits[i]) ? (value.blue() + v * y) : (value.blue() - v * y);
                break;
            case channels::RED:
                b = (bits[i]) ? (value.red() + v * y) : (value.red() - v * y);
                break;
            case channels::GREEN:
                b = (bits[i]) ? (value.green() + v * y) : (value.green() - v * y);
                break;
            case channels::Y:
                y = (bits[i]) ? (y + v * y) : (y - v * y);
                break;
            }

            if (b > 255) {
                b = 255;
            }
            if (b < 0) {
                b = 0;
            }
            switch(ch) {
            case channels::BLUE:
                container.setPixel(k, l, SET_BLUE(container.pixel(k, l), b));
                break;
            case channels::RED:
                container.setPixel(k, l, SET_RED(container.pixel(k, l), PIXELRANGE(b)));
                break;
            case channels::GREEN:
                container.setPixel(k, l, SET_GREEN(container.pixel(k, l), PIXELRANGE(b)));
                break;
            case channels::Y:
                container.setPixel(k, l, YCbCrtoRGB(y, cr, cb));
                break;
            }

            //value.setBlue(b);
            //container.setPixel(k, l, value.rgb());
        }
        //if (i < 8)
            //qDebug() << bits[i];
    }
}

void CAlgKutter::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
{
    qDebug() << "ch " << ch;
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    vector<int> seq(container.width()*container.height());
    for (int i=0; i<container.width()*container.height(); i++) {
        seq[i] = i;
    }
    random_shuffle(seq.begin(), seq.end()/*, p_myrandom*/);
    random_shuffle(seq.begin(), seq.end()/*, p_myrandom*/);
    random_shuffle(seq.begin(), seq.end()/*, p_myrandom*/);
    random_shuffle(seq.begin(), seq.end()/*, p_myrandom*/);
    random_shuffle(seq.begin(), seq.end()/*, p_myrandom*/);
    int length = keyData.length;
    QBitArray bits;
    bits.resize(length);
    int rep = container.width()*container.height() / (bits.size()*r);
    int k = 0;
    int l = 0;
    int m = 0;

    double* diffs = new double[length];
    double* signs = new double[length];
    for (int i=0; i<length; i++) {
        diffs[i] = 0;
        signs[i] = 0;
    }
    int thresh = 0;
    for (int i=0; i<length; i++) {
        double diff = 0;
        for (int t=0; t<rep; t++) {
            k = seq[m] % container.width();
            l = seq[m] / container.width();
            m++;
            if (k < len || l < len || k > container.width() - len - 1 || l > container.height() - len - 1) {
                t--;
                continue;
            }
            double avg = 0;
            for (int i = -len; i <= len; i++) {
                if (i) {
                    //avg += QColor(container.pixel(k + i, l)).blue() * (1+(len + 1 - abs(i))*0.3);
                    //avg += QColor(container.pixel(k, l + i)).blue() * (1+(len + 1 - abs(i))*0.3);
                    switch(ch) {
                    case channels::BLUE:
                        avg += QColor(container.pixel(k + i, l)).blue();
                        avg += QColor(container.pixel(k, l + i)).blue();
                        break;
                    case channels::RED:
                        avg += QColor(container.pixel(k + i, l)).red();
                        avg += QColor(container.pixel(k, l + i)).red();
                        break;
                    case channels::GREEN:
                        avg += QColor(container.pixel(k + i, l)).green();
                        avg += QColor(container.pixel(k, l + i)).green();
                        break;
                    case channels::Y:
                        avg += 0.29890 * QColor(container.pixel(k + i, l)).red() + 0.58662 * QColor(container.pixel(k + i, l)).green() + 0.11448 * QColor(container.pixel(k + i, l)).blue();
                        avg += 0.29890 * QColor(container.pixel(k, l + i)).red() + 0.58662 * QColor(container.pixel(k, l + i)).green() + 0.11448 * QColor(container.pixel(k, l + i)).blue();
                        break;
                    }

                }
            }
            /*avg += QColor(container.pixel(k+1, l+1)).blue();
            avg += QColor(container.pixel(k-1, l+1)).blue();
            avg += QColor(container.pixel(k+1, l-1)).blue();
            avg += QColor(container.pixel(k-1, l-1)).blue();*/
            //avg -= 2 * len + 1QColor(container.pixel(k, l)).blue();
            //avg /= 4 * (len + 0.3 * len * (len+1) / 2.0);
            //diff = 0;
            avg /= 4*len;
            //qDebug() << len + 0.3 * len * (len+1) / 2.0;
            switch(ch) {
            case channels::BLUE:
                diffs[i] += avg - QColor(container.pixel(k, l)).blue();
                break;
            case channels::RED:
                diffs[i] += avg - QColor(container.pixel(k, l)).red();
                break;
            case channels::GREEN:
                diffs[i] += avg - QColor(container.pixel(k, l)).green();
                break;
            case channels::Y:
                //diffs[i] += avg - (0.29890 * QColor(container.pixel(k, l)).red() + 0.58662 * QColor(container.pixel(k, l)).green() + 0.11448 * QColor(container.pixel(k, l)).blue());
                signs[i] += SIGN(avg - RGBtoY(container.pixel(k, l)));
                diffs[i] += avg - RGBtoY(container.pixel(k, l));
                break;
            }
        }
    }
    int dmin = diffs[0], dmax = diffs[0];
    for (int i=0; i<length; i++) {
        if (diffs[i] > dmax) {
            dmax = diffs[i];
        }
        if (diffs[i] < dmin) {
            dmin = diffs[i];
        }
    }
    //thresh = (dmax + dmin) / 2;
    qDebug() << dmax << " " << dmin << " " << thresh;

    int errcount = 0;

    for (int i=0; i<length; i++) {
        if (fabs(signs[i]) < 3) {
            errcount++;
        }
        if (diffs[i] < thresh) {
            bits[i] = 1;
        }
        else {
            bits[i] = 0;
        }
    }
    qDebug() << "err: " << errcount << "/" << length;
    delete [] diffs;
    delete [] signs;
    //cout << length;
    watermark = bitToByte(bits);
}


void CAlgKutter::GenKey(QByteArray& data)
{
    keyStruct keyData;
    keyData.seed = QTime(0,0,0).msecsTo(QTime::currentTime()) + QTime::currentTime().msec();
    keyData.length = data.size()*8;
    //qDebug() << keyData.seed << "\n";
    key = QByteArray((char*)&keyData, sizeof(keyData));
}

void CAlgKutter::SetParams(QString& params)
{
    CParamHelper ph;
    ph.AddToMap(&v, ph.DOUBLE, "power");
    ph.AddToMap(&r, ph.INT, "mult");
    ph.AddToMap(&len, ph.INT, "cross");
    ph.AddToMap(&ch, ph.CHANNEL, "channel");
    ph.SetParams(params);
}
