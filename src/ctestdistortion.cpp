#include "ctestdistortion.h"
#include "cimgcompare.h"
#include <QImage>
#include <QTime>
#include "util.h"
#include "cparamhelper.h"

#include <QDebug>
CTestDistortion::CTestDistortion()
{
    ch = channels::Y;
}

void CTestDistortion::Test(CAlgorithm* alg, QString& params, QString& alg_params, QByteArray& result, const QString& container_name, const QString& watermark_name, int size)
{
    QImage in;
    //QImage in2;
    in.load(container_name);
    QImage orig(in);
    QFile wm(watermark_name);
    wm.open(QIODevice::ReadOnly);
    QByteArray bytes;// = wm.readAll();
    QByteArray bytes_out;
    QBitArray bits, bits_out;
    wm.close();
    if (!size) {
        size = (in.width() / 8) * (in.height() / 8) / 8;
    }
    bytes.resize(size);
    srand(QTime::currentTime().msec());
    for (int i=0; i<size; i++) {
        bytes[i] = rand() % 256;
    }

    qDebug() << params;
    SetParams(params);
    alg->SetParams(alg_params);
    alg->GenKey(bytes);
    alg->Hide(in, bytes, alg_params);
    in.save("F:\\koh.bmp");
    //alg->Restore(in, bytes_out, alg_params);


    CImgCompare::Result res = CImgCompare::PSNR_B(orig, in, ch);
    qDebug() << ch;
    /*qDebug() << "md: " << res.md;
    qDebug() << "ad: " << res.ad;
    qDebug() << "nad: " << res.nad;
    qDebug() << "mse: " << res.mse;
    qDebug() << "nmse: " << res.nmse;
    qDebug() << "l2: " << res.l2;
    qDebug() << "snr: " << res.snr;
    qDebug() << "psnr: " << res.psnr;
    qDebug() << "image fidelity: " << res.ifid;
    qDebug() << "hs: " << res.hs;
    qDebug() << "sc: " << res.sc;
    qDebug() << "cq: " << res.cq;
    qDebug() << "nc: " << res.nc;*/
    result = SERIALIZE(res);
}

void CTestDistortion::SetParams(QString params)
{
    CParamHelper ph;
    ph.AddToMap(&ch, ph.CHANNEL, "channel");
    ph.SetParams(params);
}
