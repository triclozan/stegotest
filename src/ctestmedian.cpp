#include "ctestmedian.h"
#include <QImage>
#include <QBuffer>
#include <QDebug>
#include "CAlgorithm.h"
#include "ctransform.h"
#include <iostream>
#include <QTime>
using namespace std;

void CTestMedian::Attack(QImage& img, QByteArray& params)
{
    int N = *(int *)(params.constData());
    CTransform t;
    QString transformParams = QString('MEDIAN:') + N;
    t.performTransform(img, transformParams);
}
