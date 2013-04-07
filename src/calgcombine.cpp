#include "calgcombine.h"
#include <QTime>
#include "ctransform.h"
#include "util.h"


CAlgCombine::CAlgCombine()
{
    koh = new CAlgKoh;
    kutter = new CAlgKutter;
    fridrich = new CAlgFridrich2;
    koh_tresh = 50;
    ch = 3;
    which = 1;
    mode = 3;
    alpha = 0.01;
    gamma = 0;
    mult = 4;
}

CAlgCombine::~CAlgCombine()
{
    delete koh;
    delete kutter;
    delete fridrich;
}

void CAlgCombine::Hide(QImage& container, QByteArray& watermark, QByteArray& key)
{
    if (mode & 1) {
        koh->Hide(container, watermark);
    }
    if (mode & 2) {
        kutter->Hide(container, watermark);
    }
    if (mode & 8 || mode & 4) {
        fridrich->Hide(container, watermark);
    }
}

void CAlgCombine::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
{
    switch(which) {
    case 1:
        koh->Restore(container, watermark);
        break;
    case 2:
        kutter->Restore(container, watermark);
        break;
    case 3:
        fridrich->Restore(container, watermark);
        break;
    }
}

void CAlgCombine::GenKey(QByteArray& data)
{
    koh->GenKey(data);
    kutter->GenKey(data);
    fridrich->GenKey(data);
}

void CAlgCombine::SetParams(QString& params)
{
    CParamHelper ph;
    ph.AddToMap((void*)&koh_tresh, ph.DOUBLE, "koh_threshold");

    ph.AddToMap((void*)&power, ph.DOUBLE, "power");
    ph.AddToMap((void*)&alpha, ph.DOUBLE, "alpha");
    ph.AddToMap((void*)&gamma, ph.DOUBLE, "gamma");
    ph.AddToMap((void*)&ch, ph.CHANNEL, "channel");
    ph.AddToMap((void*)&which, ph.INT, "which");
    ph.AddToMap((void*)&mode, ph.INT, "mode");
    ph.AddToMap((void*)&mult, ph.INT, "mult");
    ph.SetParams(params);
    QString koh_params = "threshold=" + QString::number(koh_tresh) + "&channel=" + QString::number(ch);
    QString kutter_params = "power=" + QString::number(power) + "&mult=" + QString::number(mult) + "&channel=" + QString::number(ch);
    int fridrich_mode = (mode & 8) >> 2 | (mode & 4) >> 2;
    QString fridrich_params = "gamma=" + QString::number(gamma) + "&alpha=" + QString::number(alpha) + "&mode=" + QString::number(fridrich_mode) + "&channel=" + QString::number(ch);
    qDebug() << koh_params;
    qDebug() << kutter_params;
    qDebug() << params;
    qDebug() << fridrich_params;
    koh->SetParams(koh_params);
    kutter->SetParams(kutter_params);
    fridrich->SetParams(fridrich_params);
}
