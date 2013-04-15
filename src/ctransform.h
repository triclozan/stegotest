#ifndef CTRANSFORM_H
#define CTRANSFORM_H


class QImage;
class QString;
class CTransform
{
public:
    CTransform();
    void performTransform(QImage& in, QImage& out, QString& params);
    void performTransform(QImage& in, QString& params);
    void LinearCorrection(QImage& in, double** values, int channel);
    void addNoise(QImage& in, QImage& out, int level);
    void addINoise(QImage& in, QImage& out, int level);
    void addFINoise(QImage& in, QImage& out, int num, double value, int ch);
    void addBINoise(QImage& in, QImage& out, int level);
    void addGNoise(QImage& in, QImage& out, double average, double deviation = 1);
    void AMFilter(QImage& in, QImage& out, int N = 1);
    void BLFilter(QImage& in, QImage& out, double sigma, double r, int N=1);
    void GMFilter(QImage& in, QImage& out, int N = 1);
    void HMFilter(QImage& in, QImage& out, int N = 1);
    void MedFilter(QImage& in, QImage& out, int N = 1);
    void MPFilter(QImage& in, QImage& out, int N = 1);
    void AdMedFilter(QImage& in, QImage& out, int N = 1, int delta = 2);
    void Sharpen(QImage& in, QImage& out, double strength = 0.1);
    void FFT(QImage& in, QImage& out, int mode = 0);
    void Wavelet(QImage& in, QImage& out, int level);
    void HistEq(QImage& in, QImage& out);
    void AdLocalFilter(QImage& in, QImage& out, double d, int N = 1);
    void Wiener(QImage& in, QImage& out, int N = 70, int radius = 300, int stage = 2);
    void WienerGauss(QImage& in, QImage& out, int N, int radius, int stage);
};

#endif // CTRANSFORM_H
