#include "ctesttransform.h"
#include <QImage>
#include <QBuffer>
#include <QDebug>
#include "CAlgorithm.h"
#include <iostream>
#include <QTime>
#include <QBuffer>
#include "cparamhelper.h"
#include "ctransform.h"
using namespace std;

CTestTransform::CTestTransform() {}

/*void CTestTransform::Attack(QImage& img, QByteArray& params)
{
    QString s = QString::fromUtf8(params.data());
    QImage out = img;
    for (list<sTransform>::iterator it = transforms.begin(); it != transforms.end(); it++) {
        performTransform(img, out, *it);
        img = out;
    }
}*/

void CTestTransform::performTransform(QImage& in, QImage& out, sTransform& t)
{
    int level;
    fParams p;
    shParams sh;
    switch(t.id) {
    case tUNoise:
        level = *(int *)(t.params.constData());
        addNoise(in, out, level);
        break;
    case tINoise:
        level = *(int *)(t.params.constData());
        addINoise(in, out, level);
        break;
    case tBINoise:
        level = *(int *)(t.params.constData());
        addBINoise(in, out, level);
        break;
    case tAMFilter:
        p = *(fParams *)(t.params.constData());
        AMFilter(in, out, p.N);
        break;
    case tGMFilter:
        p = *(fParams *)(t.params.constData());
        GMFilter(in, out, p.N);
        break;
    case tHMFilter:
        p = *(fParams *)(t.params.constData());
        HMFilter(in, out, p.N);
        break;
    case tMedFilter:
        p = *(fParams *)(t.params.constData());
        MedFilter(in, out, p.N);
        break;
    case tAdMedFilter:
        p = *(fParams *)(t.params.constData());
        AdMedFilter(in, out, p.N);
        break;
    case tSharpen:
         sh = *(shParams *)(t.params.constData());
         //qDebug() << sh.strength;
         Sharpen(in, out, sh.strength);
        break;
    }
}

void CTestTransform::Attack(QImage& in, QString& params)
{
    CTransform t;
    t.performTransform(in, params);
    /*    CParamHelper ph;
    QString tset = params;
    ph.AddToMap(&tset, ph.STRING, "tf");
    ph.AddToMap(&tset, ph.STRING, "transform");
    ph.SetParams(params);
    qDebug() << tset;
    //qDebug() << tset;
    QImage out = in;
    QStringList t_list = tset.split(";");
    for (int i = 0; i<t_list.size(); i++) {
        QStringList options = t_list[i].split(":");
        QString cur = options[0];
        QStringList tmp = cur.split("*");
        QString str_id;
        int rep = 1, id;

        if (tmp.size() > 1) {
            rep = tmp[0].toInt();
            str_id = tmp[1];
        }
        else {
            str_id = cur;
        }
        CParamHelper ph;
        ph.Process(&id, ph.TRANSFORM, str_id);
        int level, N, quality;
        double strength;
        for (int j=0; j<rep; j++) {
            switch (id) {
            case tUNoise:
                level = options[1].toInt();
                addNoise(in, out, level);
                break;
            case tINoise:
                level = options[1].toInt();
                addINoise(in, out, level);
                break;
            case tBINoise:
                level = options[1].toInt();
                addBINoise(in, out, level);
                break;
            case tAMFilter:
                N = options[1].toInt();
                AMFilter(in, out, N);
                break;
            case tGMFilter:
                N = options[1].toInt();
                GMFilter(in, out, N);
                break;
            case tHMFilter:
                N = options[1].toInt();
                HMFilter(in, out, N);
                break;
            case tMedFilter:
                N = options[1].toInt();
                MedFilter(in, out, N);
                break;
            case tAdMedFilter:
                N = options[1].toInt();
                AdMedFilter(in, out, N);
                break;
            case tSharpen:
                 strength = options[1].toDouble();
                 //qDebug() << strength;
                 Sharpen(in, out, strength);
                 break;
            case tJpeg:
                 quality = options[1].toInt();
                 QBuffer buf;
                 in.save(&buf, "JPG", quality);
                 out.loadFromData(buf.buffer(), "JPG");
                 //out = in;
                 break;
            }
            in = out;
        }
    }*/
}


void CTestTransform::addTransform(sTransform t)
{
    transforms.push_back(t);
}
