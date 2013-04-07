#include "calglsb.h"
#include <QDebug>
#include <iostream>
using namespace std;
void CAlgLSB::Hide(QImage& container, QByteArray& watermark, QByteArray& key)
{
    QBitArray bits = byteToBit(watermark);
    int k = 0;
    int l = 0;
    for (int i=0; i<bits.size(); i++) {
        QColor value = container.pixel(k, l);
        value.setBlue((value.blue() & 0xFE) | bits[i]);
        container.setPixel(k, l, value.rgb());
        k++;
        if (k >= container.width()) {
            k = 0;
            l++;
        }
    }
}

void CAlgLSB::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
{
    int length = *((int*)key.constData());
    //qDebug() << length;
    QBitArray bits;
    bits.resize(length);
    int k = 0;
    int l = 0;
    for (int i=0; i<length; i++) {
        bits[i] = QColor(container.pixel(k, l)).blue() & 1;
        k++;
        if (k >= container.width()) {
            k = 0;
            l++;
        }
    }
    watermark = bitToByte(bits);
}


void CAlgLSB::GenKey(QByteArray& data)
{
    //qDebug("www");
    int length = data.size()*8;
    //cout << length;
    key = QByteArray((char*)(&length), sizeof(int));
}
