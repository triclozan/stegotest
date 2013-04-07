#include "ctestmedian.h"
#include <QImage>
#include <QBuffer>
#include <QDebug>
#include "CAlgorithm.h"
#include <iostream>
#include <QTime>
using namespace std;

/*void CTestMedian::Test(CAlgorithm* alg, QByteArray& params, QByteArray& alg_params, sResults& result, const QString& container_name, const QString& watermark_name)
{
    QImage in;
    //QImage in2;
    QBuffer buf;
    in.load(container_name);
    //QImage orig(in);
    QFile wm(watermark_name);
    wm.open(QIODevice::ReadOnly);
    QByteArray bytes = wm.readAll();
    QByteArray bytes_out;
    QBitArray bits, bits_out;
    wm.close();
    alg->GenKey(watermark_name);
    alg->Hide(in, bytes, alg_params);

    int N = *(int *)(params.constData());
    QImage out = in;
    MedFilter(in, out);
    alg->Restore(out, bytes_out, alg_params);
    out.save("F:\\res.bmp");

    int count = 0;
    for (int i=0; i< bytes.size(); i++) {
        if (bytes[i] == bytes_out[i]) {
            count++;
        }
    }

    result.ByER = ((double)count) / bytes.size();
    bits = alg->byteToBit(bytes);
    bits_out = alg->byteToBit(bytes_out);
    count = 0;
    for (int i=0; i< bits.size(); i++) {
        if (bits[i] == bits_out[i]) {
            count++;
        }
    }

    result.BER = ((double)count) / bits.size();
}*/

void CTestMedian::Attack(QImage& img, QByteArray& params)
{
    int N = *(int *)(params.constData());
    QImage out = img;
    MedFilter(img, out);
    img = out;
}
