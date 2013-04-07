#include "ctestkoh.h"
#include <QImage>
#include <QBuffer>
#include <QDebug>
#include "CAlgorithm.h"
#include "calgkoh.h"
#include <iostream>
#include <QTime>
#include "util.h"
using namespace std;

QImage CTestKoh::Test(const QString& container_name, const QString& watermark_name)
{
    CAlgorithm* alg = new CAlgKoh;
    QImage in, in2, res;
    QBuffer buf;
    in.load(container_name);
    in2 = in;
    int b_x = in.width() / 8;
    int b_y = in.height() / 8;
    double** mask = new double*[b_y];
    for (int i=0; i<b_y; i++) {
        mask[i] = new double[b_x];
    }
    for (int i=0; i<b_x; i++) {
        for (int j=0; j<b_y; j++) {
            mask[i][j] = 0;
        }
    }
    res = QImage(in.width(), in.height(), QImage::Format_ARGB32);
    int size = (in.width() / 8) * (in.height() / 8) / 8;
    QByteArray bytes = QByteArray(size, 0);
    QByteArray bytes_out;
    QBitArray bits, bits_out;

    srand(QTime(0,0,0).msecsTo(QTime::currentTime()) + QTime::currentTime().msec());
    for (int i=0; i<size; i++) {
        bytes[i] = 0;
    }

    alg->GenKey(bytes);
    QByteArray params, alg_params;
    int tresh = 30;
    alg_params = QByteArray((char*)&tresh, sizeof(tresh));
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            res.setPixel(i, j, 0);
        }
    }

    for (int i=100; i>=80; i-=1) {
        for (int j=0; j<2; j++) {
            qDebug() << j << " " << i;
            for (int k=0; k<size; k++) {
                bytes[k] = 255 - bytes[k];// - bytes[i];
            }
            params = QByteArray((char*)&i, sizeof(i));
            in = in2;
            alg->Hide(in, bytes, alg_params);
            Attack(in, params);
            alg->Restore(in, bytes_out, alg_params);
            bits = alg->byteToBit(bytes);
            bits_out = alg->byteToBit(bytes_out);
            int count = 0;
            for (int i=0; i< bits.size(); i++) {
                if (bits[i] != bits_out[i]) {
                    count++;
                    mask[i%b_x][i/b_x]++;
                }
            }
            qDebug() << "ber: " << (bits.size() - count) / (double)bits.size();
        }
    }

    int mmax = 0;

    for (int i=0; i<b_x; i++) {
        for (int j=0; j<b_y; j++) {
            if (mask[i][j] > mmax) {
                mmax = mask[i][j];
            }
        }
    }
    for (int i=0; i<b_x; i++) {
        for (int j=0; j<b_y; j++) {
            double d = ROUND((mask[i][j])  * 255 / (double)mmax);
            int b = PIXELRANGE(d);
            if (b < 100) {
                mask[i][j] = 0xFF << 24;
            }
            else {
                mask[i][j] = PIXEL(b, b, b);
            }
        }
    }
    for (int i=0; i<b_x; i++) {
        for (int j=0; j<b_y; j++) {
            for (int k=0; k<8; k++) {
                for (int l=0; l<8; l++) {
                    res.setPixel(i*8+k, j*8+l, mask[i][j]);
                }
            }

        }
    }
    return res;
}

void CTestKoh::Attack(QImage& img, QByteArray& params)
{
    QBuffer buf;
    int quality = *(int *)(params.constData());
    img.save(&buf, "JPG", quality);
    img.loadFromData(buf.buffer(), "JPG");
}
