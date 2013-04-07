#include "ctestnoise.h"
#include <QImage>
#include <QBuffer>
#include <QDebug>
#include "CAlgorithm.h"
#include <iostream>
#include <QTime>
using namespace std;

void CTestNoise::Test(CAlgorithm* alg, QByteArray& params, QByteArray& alg_params, sResults& result, const QString& container_name, const QString& watermark_name)
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

    int level = *(int *)(params.constData());
    //qDebug() << level;

    QImage out = in;
    addNoise(in, out, level);
    //HMFilter(in, out);
    alg->Restore(out, bytes_out, alg_params);
    out.save("F:\\res.bmp");

    int count = 0;
    for (int i=0; i< bytes.size(); i++) {
        if (bytes[i] == bytes_out[i]) {
            count++;
        }
    }
    //cout << count << "\n";
    //result += QString("ByER: ") + QString::number(((double)count) / bytes.size());
    result.ByER = ((double)count) / bytes.size();
    bits = alg->byteToBit(bytes);
    bits_out = alg->byteToBit(bytes_out);
    count = 0;
    for (int i=0; i< bits.size(); i++) {
        //qDebug() << int(bits[i]) << int(bits_out[i]);
        if (bits[i] == bits_out[i]) {
            count++;
        }
    }

    result.BER = ((double)count) / bits.size();
    //result += QString::number(((double)count) / bits.size());
    //CImgCompare::PSNR_B(orig, in);
}
