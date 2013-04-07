#include "calgpri.h"
#include <QDebug>
#include <iostream>
#include <QTime>
using namespace std;

void CAlgPRI::Hide(QImage& container, QByteArray& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    QBitArray bits = byteToBit(watermark);
    int k = 0;
    int l = 0;
    int m = 0;
    for (int i=0; i<bits.size(); i++) {
        k = m % container.width();
        l = m / container.width();
        QColor value = container.pixel(k, l);
        value.setBlue((value.blue() & 0xFE) | bits[i]);
        container.setPixel(k, l, value.rgb());
        m += (1 + qrand() % 8);
        //cout << m;
    }
}

void CAlgPRI::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    int length = keyData.length;
    QBitArray bits;
    bits.resize(length);
    int k = 0;
    int l = 0;
    int m = 0;
    for (int i=0; i<length; i++) {
        k = m % container.width();
        l = m / container.width();
        bits[i] = QColor(container.pixel(k, l)).blue() & 1;
        m += (1 + qrand() % 8);
    }
    //cout << length;
    watermark = bitToByte(bits);
}


void CAlgPRI::GenKey(QByteArray& data)
{
    keyStruct keyData;
    keyData.seed = QTime::currentTime().msec();
    keyData.length = data.size()*8;
    key = QByteArray((char*)&keyData);
}
