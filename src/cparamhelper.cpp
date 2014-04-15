#include "cparamhelper.h"
#include <QStringList>
#include <QDebug>

#include "util.h"

CParamHelper::CParamHelper()
{
}

void CParamHelper::AddToMap(void* ptr, int type, QString name)
{
    paramMap.push_back(sParam(ptr, type, name));
}

void CParamHelper::SetParams(QString& params)
{
    SetParams(params, "&");
}

void CParamHelper::Clear()
{
    paramMap.clear();
}

void CParamHelper::SetParams(QString& params, QString param_sep)
{
    QStringList pairs = params.split(param_sep);
    QStringList names, values;
    for (int i=0; i<pairs.size(); i++) {
        QStringList tmp = pairs[i].split("=");
        if (tmp.size() < 2) {
            continue;
        }
        names.push_back(tmp[0].toLower());
        values.push_back(tmp[1]);
    }
    for (int i=0; i<paramMap.size(); i++) {
        for (int j=0; j<names.size(); j++) {
            if (paramMap[i].name == names[j]) {
                Process(paramMap[i].ptr, paramMap[i].type, values[j]);
            }
        }
    }
}

void CParamHelper::Process(void* ptr, int type, QString value)
{
    switch(type) {
    case INT:
        *(int*)(ptr) = value.toInt();
        break;
    case DOUBLE:
        *(double*)(ptr) = value.toDouble();
        break;
    case STRING:
        *(QString*)(ptr) = value;
        break;
    case CHANNEL:
        //qDebug() << "WWW";
        value = value.toUpper();
        if (value == "BLUE") {
            *(int*)(ptr) = channels::BLUE;
        }
        else if (value == "GREEN") {
             *(int*)(ptr) = channels::GREEN;
        }
        else if (value == "RED") {
             *(int*)(ptr) = channels::RED;
        }
        else if (value == "Y") {
             *(int*)(ptr) = channels::Y;
        }
        else {
            *(int*)(ptr) = value.toInt();
        }
        break;

    case TRANSFORM:
        value = value.toUpper();
        //qDebug() << "WWW";
        if (value == "JPEG") {
            *(int*)(ptr) = tJpeg;
        }
        else if (value == "WIENER") {
            *(int*)(ptr) = tWiener;
        }
        else if (value == "WAVELET_NOISE") {
            *(int*)(ptr) = tWavelet;
        }
        else if (value == "FURIE_NOISE") {
            *(int*)(ptr) = tFINoise;
        }
        else if (value == "EQ_HISTOGRAM") {
            *(int*)(ptr) = tHisteq;
        }
        else if (value == "BLUR") {
             *(int*)(ptr) = tAMFilter;
        }
        else if (value == "GBLUR") {
             *(int*)(ptr) = tGBlur;
        }
        else if (value == "GEOMETRIC_BLUR") {
             *(int*)(ptr) = tGMFilter;
        }
        else if (value == "HARMONIC_BLUR") {
             *(int*)(ptr) = tHMFilter;
        }
        else if (value == "FOURIER_NOISE") {
             *(int*)(ptr) = tFINoise;
        }
        else if (value == "BILATERAL") {
             *(int*)(ptr) = tBLFilter;
        }
        else if (value == "ADAPTIVE_LOCAL") {
             *(int*)(ptr) = tAdLocalFilter;
        }
        else if (value == "MID_POINT") {
             *(int*)(ptr) = tMPFilter;
        }
        else if (value == "BILATERAL") {
             *(int*)(ptr) = tBLFilter;
        }
        else if (value == "MEDIAN") {
             *(int*)(ptr) = tMedFilter;
        }
        else if (value == "ADAPTIVE_MEDIAN") {
             *(int*)(ptr) = tAdMedFilter;
        }
        else if (value == "UNIFORM_NOISE") {
             *(int*)(ptr) = tUNoise;
        }
        else if (value == "GAUSS_NOISE") {
             *(int*)(ptr) = tGNoise;
        }
        else if (value == "IMPULSE_NOISE") {
             *(int*)(ptr) = tINoise;
        }
        else if (value == "SHARPEN") {
             *(int*)(ptr) = tSharpen;
        }
        else {
            *(int*)(ptr) = value.toInt();
        }
        break;
    }
}
