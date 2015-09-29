#include "ctestnoise.h"
#include <QImage>
#include <QBuffer>
#include <QDebug>
#include "CAlgorithm.h"
#include "ctransform.h"
#include <iostream>
#include <QTime>
using namespace std;

void CTestNoise::Attack(QImage& img, QByteArray& params)
{
    int N = *(int *)(params.constData());
    CTransform t;
    QString transformParams = QString('UNIFORM_NOISE:') + N;
    t.performTransform(img, transformParams);
}

