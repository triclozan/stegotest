#ifndef CALGORITHM_H
#define CALGORITHM_H
#include <QString>
#include <QImage>
#include <QBitArray>
#include <QFile>
#include <QColor>
#include <QDebug>
#include "cparamhelper.h"

class CAlgorithm
{
public:
    //CAlgorithm(int a);
    //virtual ~CAlgorithm();
    virtual void Hide(QImage& container, QByteArray& watermark, QByteArray& key)=0;
    virtual void Restore(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual double Detect(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual bool DetectStrict(QImage& container, QByteArray& watermark, QByteArray& key, int thresh=0.7);
    void Hide(QImage& container, QByteArray& watermark);
    void Restore(QImage& container, QByteArray& watermark);
    void Hide(QImage& container, QByteArray& watermark, QString& p);
    void Restore(QImage& container, QByteArray& watermark, QString& p);
    void SetParams(QByteArray& p);
    virtual void SetParams(QString& params);
    QByteArray ExportKey();
    virtual void GenKey(QByteArray& data);
    void GenKey(const QString& watermark_name);
    void HideToFile(const QString& container_name, const QString& watermark_name, const QString& result_name, QByteArray& key);
    void RestoreFromFile(const QString& container_name, const QString& watermark_name, QByteArray& key);
    void HideToFile(const QString& container_name, const QString& watermark_name, const QString& result_name);
    void RestoreFromFile(const QString& container_name, const QString& watermark_name);
    QByteArray bitToByte(QBitArray bits);
    QBitArray byteToBit(QByteArray bytes);
protected:
    QByteArray key;  
    QString params;
};

#endif // CALGORITHM_H
