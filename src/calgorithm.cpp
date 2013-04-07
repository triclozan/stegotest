#include "calgorithm.h"
#include <QFile>
#include <QImage>
#include <QStringList>

/*CAlgorithm::CAlgorithm(int a) {}*/

void CAlgorithm::HideToFile(const QString& container_name, const QString& watermark_name, const QString& result_name, QByteArray& key)
{
    QImage in;
    in.load(container_name);
    QFile wm(watermark_name);
    wm.open(QIODevice::ReadOnly);
    QByteArray bytes = wm.readAll();
    wm.close();
    SetParams(params);
    Hide(in, bytes, key);
    in.save(result_name);
}

CAlgorithm::~CAlgorithm() {
}

void CAlgorithm::HideToFile(const QString& container_name, const QString& watermark_name, const QString& result_name)
{
    HideToFile(container_name, watermark_name, result_name, key);
}

void CAlgorithm::RestoreFromFile(const QString& container_name, const QString& watermark_name)
{
    RestoreFromFile(container_name, watermark_name, key);
}

void CAlgorithm::RestoreFromFile(const QString& container_name, const QString& watermark_name, QByteArray& key)
{
    QImage in;
    in.load(container_name);
    QFile wm(watermark_name);
    wm.open(QIODevice::WriteOnly);
    QByteArray bytes;
    SetParams(params);
    Restore(in, bytes, key);
    wm.write(bytes);
    wm.close();
}

QByteArray CAlgorithm::bitToByte(QBitArray bits) {
    QByteArray byteArray;
    unsigned char c;
    for(int j=0; j<bits.size()/8; j++){
        c = 0;
        int k = 0;
        for(int i=j*8; i<j*8+8; i++){
            if (bits.testBit(i)){
                c |= ( 1 << k);
            }
            k++;
        }
        byteArray.append(c);
    }
    return byteArray;
}

QBitArray CAlgorithm::byteToBit(QByteArray bytes) {
    QBitArray outArray;
    outArray.resize(bytes.size()*8);
    for(int i=0;i<outArray.size();i++){
        if(bytes[i/8]>>i%8 & 1){outArray.setBit(i,true);}
    }
    return outArray;
}

void CAlgorithm::GenKey(QByteArray& data)
{
    return;
}

void CAlgorithm::GenKey(const QString& watermark_name)
{
    QFile wm(watermark_name);
    wm.open(QIODevice::ReadOnly);
    QByteArray bytes = wm.readAll();
    wm.close();
    GenKey(bytes);
    return;
}

void CAlgorithm::Hide(QImage& container, QByteArray& watermark)
{
    Hide(container, watermark, key);
}

void CAlgorithm::Hide(QImage& container, QByteArray& watermark, QString& p)
{
    SetParams(p);
    Hide(container, watermark, key);
}

void CAlgorithm::Restore(QImage& container, QByteArray& watermark)
{
    Restore(container, watermark, key);
}

void CAlgorithm::Restore(QImage& container, QByteArray& watermark, QString& p)
{
    SetParams(p);
    Restore(container, watermark, key);
}

QByteArray CAlgorithm::ExportKey()
{
    return key;
}

void CAlgorithm::SetParams(QByteArray& p)
{
    params = p;
}

void CAlgorithm::SetParams(QString& p)
{
    params = p;
}

void CAlgorithm::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
{

}

double CAlgorithm::Detect(QImage& container, QByteArray& watermark, QByteArray& key)
{
    QByteArray res_watermark;
    Restore(container, res_watermark, key);
    QBitArray bits, bits_out;
    bits = byteToBit(watermark);
    bits_out = byteToBit(res_watermark);
    int count = 0;
    for (int i=0; i< bits_out.size(); i++) {
        if (bits[i] == bits_out[i]) {
            count++;
        }
    }
    return (double)count / bits.size();
}

bool CAlgorithm::DetectStrict(QImage& container, QByteArray& watermark, QByteArray& key, int thresh)
{
    if (Detect(container, watermark, key) > thresh) {
        return true;
    }
    else {
        return false;
    }
}
