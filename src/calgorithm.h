#ifndef CALGORITHM_H
#define CALGORITHM_H
#include <QString>
#include <QImage>
#include <QBitArray>
#include <QVector>
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
    void Hide(QImage& container, QByteArray& watermark);
    void Hide(QImage& container, QByteArray& watermark, QString& p);
    void HideToFile(const QString& container_name, const QString& watermark_name, const QString& result_name, QByteArray& key);
    void HideToFile(const QString& container_name, const QString& watermark_name, const QString& result_name);

    virtual void Restore(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual void Restore(QImage& container, QVector<double>& res, QByteArray& key);
    void Restore(QImage& container, QByteArray& watermark);
    void Restore(QImage& container, QVector<double> &res);
    void Restore(QImage& container, QByteArray& watermark, QString& p);
    void RestoreFromFile(const QString& container_name, const QString& watermark_name, QByteArray& key);
    void RestoreFromFile(const QString& container_name, const QString& watermark_name);

    virtual double Detect(QImage& container, QByteArray& watermark, QByteArray& key);
    virtual bool DetectStrict(QImage& container, QByteArray& watermark, QByteArray& key, int thresh=0.7);

    QByteArray ExportKey();
    virtual void GenKey(QByteArray& data);
    void GenKey(const QString& watermark_name);   
   
    void SetParams(QByteArray& p);
    virtual void SetParams(QString& params);

    static QByteArray bitToByte(QBitArray bits);
    static QBitArray byteToBit(QByteArray bytes);

    virtual ~CAlgorithm();
protected:
    QByteArray key;  
    QString params;
};

#endif // CALGORITHM_H
