#include "ctransform.h"
#include <QImage>
#include <QTime>
#include <QColor>
#include <QDebug>
#include <QBuffer>
#include "RNG/stocc.h"
#include "math.h"
#include "util.h"
#include "cparamhelper.h"
#include "FFT/fftw3.h"
#include "dct.h"
//#include "FFT/wavelet2d.h"
CTransform::CTransform()
{
}

void CTransform::performTransform(QImage& in, QImage& out, QString& params)
{
    out = in;
    performTransform(out, params);
}

void CTransform::performTransform(QImage& in, QString& params)
{
    CParamHelper ph;
    QString tset = params;
    ph.AddToMap(&tset, ph.STRING, "tf");
    ph.AddToMap(&tset, ph.STRING, "transform");
    ph.SetParams(tset);
    qDebug() << tset;
    //qDebug() << tset;
    QImage out = in;
    QImage *src = &in, *dst = &out;
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
        qDebug() << id;
        int level, N, quality, delta, ch;

        double strength, d;
        for (int j=0; j<rep; j++) {
            switch (id) {
            case tUNoise:
                level = options[1].toInt();
                addNoise(*src, *dst, level);
                break;
            case tINoise:
                level = options[1].toInt();
                addINoise(*src, *dst, level);
                break;
            case tFINoise:
                addFINoise(*src, *dst, options[1].toInt(), options[2].toDouble(), options[3].toInt());
                break;
            case tGNoise:
                if (options.size() > 2) {
                    addGNoise(*src, *dst, options[1].toDouble(), options[2].toDouble());
                }
                else {
                    addGNoise(*src, *dst, options[1].toDouble());
                }
                break;
            case tBINoise:
                level = options[1].toInt();
                addBINoise(*src, *dst, level);
                break;
            case tAdLocalFilter:
                d = options[1].toDouble();
                N = options[2].toInt();
                AdLocalFilter(*src, *dst, d, N);
                break;
            case tAMFilter:
                N = options[1].toInt();
                AMFilter(*src, *dst, N);
                break;
            case tGBlur:
                d = options[1].toDouble();
                GaussBlur(*src, *dst, d);
                break;
            case tBLFilter:
                if (options.size() > 3) {
                    BLFilter(*src, *dst, options[1].toDouble(), options[2].toDouble(), options[3].toInt());
                }
                else {
                    BLFilter(*src, *dst, options[1].toDouble(), options[2].toDouble());
                }
                break;
            case tGMFilter:
                N = options[1].toInt();
                GMFilter(*src, *dst, N);
                break;
            case tMPFilter:
                N = options[1].toInt();
                AMFilter(*src, *dst, N);
                break;
            case tFFT:
                if (options.size() > 1) {
                    N = options[1].toInt();
                    FFT(*src, *dst, N);
                }
                else {
                    FFT(*src, *dst);
                }
                break;
            case tWavelet:
                Wavelet(*src, *dst, options[1].toInt());
                break;
            case tWiener:
                if (options.size() > 4) {
                    Wiener(*src, *dst, options[1].toInt(), options[2].toDouble(), options[3].toInt(), options[3].toInt());
                }
                if (options.size() > 3) {
                    Wiener(*src, *dst, options[1].toInt(), options[2].toDouble(), options[3].toInt());
                }
                else if (options.size() > 2) {
                    Wiener(*src, *dst, options[1].toInt(), options[2].toDouble());
                }
                else if (options.size() > 1) {
                    Wiener(*src, *dst, options[1].toInt());
                }
                else {
                    Wiener(*src, *dst);
                }

                break;
            case tHMFilter:
                N = options[1].toInt();
                HMFilter(*src, *dst, N);
                break;
            case tMedFilter:
                N = options[1].toInt();
                if (options.size() > 2) {
                    delta = options[2].toInt();
                    MedFilter(*src, *dst, N, delta);
                }
                else {
                    MedFilter(*src, *dst, N);
                }
                break;
            case tHisteq:
                HistEq(*src, *dst);
                break;
            case tAdMedFilter:
                N = options[1].toInt();
                if (options.size() > 3) {
                    delta = options[2].toInt();
                    AdMedFilter(*src, *dst, N, delta, options[3].toInt());
                }
                else if (options.size() > 2) {
                    delta = options[2].toInt();
                    AdMedFilter(*src, *dst, N, delta);
                }
                else {
                    AdMedFilter(*src, *dst, N);
                }
                break;
            case tSharpen:
                 strength = options[1].toDouble();
                 //qDebug() << strength;
                 Sharpen(*src, *dst, strength);
                 break;
            case tJpeg:
                 quality = options[1].toInt();
                 QBuffer buf;
                 (*src).save(&buf, "JPG", quality);
                 (*dst).loadFromData(buf.buffer(), "JPG");
                 //out = in;
                 break;
            }
            QImage *tmp = src;
            src = dst;
            dst = tmp;
            //in = out;
        }
    }
    if (src != &in) {
        in = (*src);
    }
}

void CTransform::LinearCorrection(QImage& in, double** values, int channel)
{
    int w = in.width();
    int h = in.width();
    double max = 0;
    double min = values[0][0];
    for (int i=0; i<w; i++) {
        for (int j = 0; j<w; j++) {
            if (values[i][j] > max) {
                max = values[i][j];
            }
            if (values[i][j] < min) {
                min = values[i][j];
            }
        }
    }
    double d = max - min;
    for (int i=0; i<w; i++) {
        for (int j = 0; j<w; j++) {
            in.setPixel(i, j, in.pixel(i, j) & 0xFFFFFF00 | ROUND( ((values[i][j] - min) / d) * 255 ));
            if (values[i][j] > max) {
                max = values[i][j];
            }
        }
    }
}

int compare_int_val (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}



void CTransform::addNoise(QImage& in, QImage& out, int level)
{
    srand(QTime::currentTime().msec());
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            QColor color = QColor(in.pixel(i, j));
            int r = color.red() + rand() % (2*level + 1) - level;
            int g = color.green() + rand() % (2*level + 1) - level;
            int b = color.blue() + rand() % (2*level + 1) - level;
            color.setBlue(PIXELRANGE(ROUND(b)));
            color.setRed(PIXELRANGE(ROUND(r)));
            color.setGreen(PIXELRANGE(ROUND(g)));
            out.setPixel(i, j, color.rgb());
        }
    }
}

void CTransform::addGNoise(QImage& in, QImage& out, double average, double deviation)
{
    StochasticLib1 rng(QTime::currentTime().msec());
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            QColor color = QColor(in.pixel(i, j));
            int r = color.red() + rng.Normal(average, deviation);
            int g = color.green() + rng.Normal(average, deviation);
            int b = color.blue() + rng.Normal(average, deviation);
            color.setBlue(PIXELRANGE(ROUND(b)));
            color.setRed(PIXELRANGE(ROUND(r)));
            color.setGreen(PIXELRANGE(ROUND(g)));
            out.setPixel(i, j, color.rgb());
        }
    }
}

void CTransform::addINoise(QImage& in, QImage& out, int level)
{
    srand(QTime::currentTime().msec());
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            QColor color = QColor(in.pixel(i, j));
            if (rand() % 100 < level) {
                color.setBlue(255);
                color.setRed(255);
                color.setGreen(255);
                out.setPixel(i, j, color.rgb());
            }
            else {
                out.setPixel(i, j, in.pixel(i, j));
            }
        }
    }
}

void CTransform::addBINoise(QImage& in, QImage& out, int level)
{
    srand(QTime::currentTime().msec());
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            QColor color = QColor(in.pixel(i, j));
            int a = rand() % 100;
            if (a < level) {
                color.setBlue(255);
                color.setRed(255);
                color.setGreen(255);
                out.setPixel(i, j, color.rgb());
            }
            else if (a >= level && a < 2 * level){
                color.setBlue(0);
                color.setRed(0);
                color.setGreen(0);
                out.setPixel(i, j, color.rgb());
            }
            else {
                out.setPixel(i, j, in.pixel(i, j));
            }
        }
    }
}

void CTransform::AMFilter(QImage& in, QImage& out, int N)
{
    srand(QTime::currentTime().msec());
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            int count = 0;
            int r = 0, g = 0, b = 0;
            for (int k=-N; k<=N; k++) {
                for (int l=-N; l<=N; l++) {
                    if (k + i < 0 || k + i >= in.width() || l+j < 0 || l + j >= in.height()) {
                        continue;
                    }
                    count++;
                    r += RED(in.pixel(i+k, j+l));
                    g += GREEN(in.pixel(i+k, j+l));
                    b += BLUE(in.pixel(i+k, j+l));
                }
            }

            /*for (int k=0; k<=N; k++) {
                    if (k + i < 0 || k + i >= in.width() || k + j < 0 || k + j >= in.height()) {
                        continue;
                    }
                    count++;
                    r += RED(in.pixel(i+k, j+k));
                    g += GREEN(in.pixel(i+k, j+k));
                    b += BLUE(in.pixel(i+k, j+k));
            }*/
            int p = PIXEL(r/count, g/count, b/count);
            out.setPixel(i, j, p);
        }
    }
}

void CTransform::AdLocalFilter(QImage& in, QImage& out, double d, int N)
{
    srand(QTime::currentTime().msec());
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            int count = 0;
            double r = 0, g = 0, b = 0, r2 = 0, g2 = 0, b2 = 0;
            int rv, gv, bv;
            for (int k=-N; k<=N; k++) {
                for (int l=-N; l<=N; l++) {
                    if (k + i < 0 || k + i >= in.width() || l+j < 0 || l + j >= in.height()) {
                        continue;
                    }
                    count++;
                    r += RED(in.pixel(i+k, j+l));
                    r2 += RED(in.pixel(i+k, j+l)) * RED(in.pixel(i+k, j+l));
                    g += GREEN(in.pixel(i+k, j+l));
                    g2 += GREEN(in.pixel(i+k, j+l)) * GREEN(in.pixel(i+k, j+l));
                    b += BLUE(in.pixel(i+k, j+l));
                    b2 += BLUE(in.pixel(i+k, j+l)) * BLUE(in.pixel(i+k, j+l));
                }
            }
            r /= count;
            r2 /= count;
            g /= count;
            g2 /= count;
            b /= count;
            b2 /= count;
            r2 -= r * r;
            g2 -= g * g;
            b2 -= b * b;
            rv = RED(in.pixel(i, j));
            gv = GREEN(in.pixel(i, j));
            bv = BLUE(in.pixel(i, j));
            r = rv - ((d<=r2?d:r2) / r2) * (rv - r);
            g = gv - ((d<=g2?d:g2) / g2) * (gv - g);
            b = bv - ((d<=b2?d:b2) / b2) * (bv - b);
            rv = PIXELRANGE(ROUND(r));
            gv = PIXELRANGE(ROUND(g));
            bv = PIXELRANGE(ROUND(b));
            /*for (int k=0; k<=N; k++) {
                    if (k + i < 0 || k + i >= in.width() || k + j < 0 || k + j >= in.height()) {
                        continue;
                    }
                    count++;
                    r += RED(in.pixel(i+k, j+k));
                    g += GREEN(in.pixel(i+k, j+k));
                    b += BLUE(in.pixel(i+k, j+k));
            }*/
            int p = PIXEL(rv, gv, bv);
            out.setPixel(i, j, p);
        }
    }
}

void CTransform::BLFilter(QImage& in, QImage& out, double sigma, double sigma_r, int N)
{
    srand(QTime::currentTime().msec());
    double** gauss = new double* [2 * N + 1];
    for (int i=-N; i<=N; i++) {
        gauss[i+N] = new double[2 * N + 1];
        for (int j=-N; j<=N; j++) {
            gauss[i+N][j+N] = exp(-0.5*(POW2(i)+POW2(j)) / POW2(sigma_r));
        }
    }
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            double total_weight = 0;
            double r = 0, g = 0, b = 0;
            for (int k=-N; k<=N; k++) {
                for (int l=-N; l<=N; l++) {
                    if (k + i < 0 || k + i >= in.width() || l+j < 0 || l + j >= in.height()) {
                        continue;
                    }
                    double diff = exp(- 0.5 * (POW2(RED(in.pixel(i+k, j+l)) - RED(in.pixel(i, j))) +
                                        POW2(GREEN(in.pixel(i+k, j+l)) - GREEN(in.pixel(i, j))) +
                                        POW2(BLUE(in.pixel(i+k, j+l)) - BLUE(in.pixel(i, j)))
                                      ) / POW2(sigma));
                    double weight = diff * gauss[k+N][l+N];
                    total_weight += weight;
                    r += RED(in.pixel(i+k, j+l)) * weight;
                    g += GREEN(in.pixel(i+k, j+l)) * weight;
                    b += BLUE(in.pixel(i+k, j+l)) * weight;
                }
            }
            double rdiff = RED(in.pixel(i, j)) - r/total_weight;
            int p = PIXEL(PIXELRANGE(ROUND(r/total_weight)), PIXELRANGE(ROUND(g/total_weight)), PIXELRANGE(ROUND(b/total_weight)));
            out.setPixel(i, j, p);
        }
    }
    for (int i=0; i<N; i++) {
        delete [] gauss[i];
    }
    delete [] gauss;
}

void CTransform::GMFilter(QImage& in, QImage& out, int N)
{
    srand(QTime::currentTime().msec());
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            int count = 0;
            double r = 1, g = 1, b = 1;
            for (int k=-N; k<=N; k++) {
                for (int l=-N; l<=N; l++) {
                    if (k + i < 0 || k + i >= in.width() || l+j < 0 || l + j >= in.height()) {
                        continue;
                    }
                    count++;
                    r *= RED(in.pixel(i+k, j+l));
                    g *= GREEN(in.pixel(i+k, j+l));
                    b *= BLUE(in.pixel(i+k, j+l));
                }
            }
            int p = PIXEL(PIXELRANGE(ROUND(pow(r, 1.0 / count))), PIXELRANGE(ROUND(pow(g, 1.0 / count))), PIXELRANGE(ROUND(pow(b, 1.0 / count))));
            out.setPixel(i, j, p);
        }
    }
}


void CTransform::HMFilter(QImage& in, QImage& out, int N)
{
    srand(QTime::currentTime().msec());
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            int count = 0;
            double r = 0, g = 0, b = 0;
            double r2 = 0, g2 = 0, b2 = 0;
            double q = -1;
            for (int k=-N; k<=N; k++) {
                for (int l=-N; l<=N; l++) {
                    if (k + i < 0 || k + i >= in.width() || l+j < 0 || l + j >= in.height()) {
                        continue;
                    }
                    count++;
                    r += pow((RED(in.pixel(i+k, j+l))), q);
                    g += pow((GREEN(in.pixel(i+k, j+l))), q);
                    b += pow((BLUE(in.pixel(i+k, j+l))), q);
                    r2 += pow((RED(in.pixel(i+k, j+l))), q + 1);
                    g2 += pow((GREEN(in.pixel(i+k, j+l))), q + 1);
                    b2 += pow((BLUE(in.pixel(i+k, j+l))), q + 1);
                }
            }
            int p = PIXEL(PIXELRANGE(ROUND(r2 / r)), PIXELRANGE(ROUND(g2 / g)), PIXELRANGE(ROUND(b2 / b)));
            out.setPixel(i, j, p);
        }
    }
}

/*
    int W = 2*N + 1;
    srand(QTime::currentTime().msec());
    int *ar = new int[W*W];
    int *ag = new int[W*W];
    int *ab = new int[W*W];
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            int count = 0;
            int r = 0, g = 0, b = 0;
            if (1==1 || RED(in.pixel(i, j)) > th && GREEN(in.pixel(i, j)) > th && BLUE(in.pixel(i, j)) > th) {
                for (int k=-N; k<=N; k++) {
                    for (int l=-N; l<=N; l++) {
                        if (k + i < 0 || k + i >= in.width() || l+j < 0 || l + j >= in.height()) {
                            continue;
                        }
                        ar[count] = RED(in.pixel(i+k, j+l));
                        ag[count] = GREEN(in.pixel(i+k, j+l));
                        ab[count] = BLUE(in.pixel(i+k, j+l));
                        count++;
                    }
                }
                qsort(ar, count, sizeof(int), compare_int_val);
                qsort(ag, count, sizeof(int), compare_int_val);
                qsort(ab, count, sizeof(int), compare_int_val);
                if (count % 2) {
                    r = PIXELRANGE(ROUND(ar[count / 2]));
                    g = PIXELRANGE(ROUND(ag[count / 2]));
                    b = PIXELRANGE(ROUND(ab[count / 2]));
                }
                else {
                    r = PIXELRANGE(ROUND( (ar[count / 2] + ar[count / 2 - 1]) / 2 ));
                    g = PIXELRANGE(ROUND( (ag[count / 2] + ag[count / 2 - 1]) / 2 ));
                    b = PIXELRANGE(ROUND( (ab[count / 2] + ab[count / 2 - 1]) / 2 ));
                }
                out.setPixel(i, j, PIXEL(PIXELRANGE(ROUND(r)), PIXELRANGE(ROUND(g)), PIXELRANGE(ROUND(b))));
            }
            else {
                out.setPixel(i, j, in.pixel(i, j));
            }
        }
    }
    delete [] ar;
    delete [] ag;
    delete [] ab;
*/

void CTransform::MedFilter(QImage& in, QImage& out, int N, int th)
{
    int W = 2*N + 1;
    srand(QTime::currentTime().msec());
    int *ar = new int[W*W];
    int *ag = new int[W*W];
    int *ab = new int[W*W];
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            int count = 0;
            int r = 0, g = 0, b = 0;
            int c1 = RED(in.pixel(i, j));
            int c2 = GREEN(in.pixel(i, j));
            int c3 = BLUE(in.pixel(i, j));
            if (i == j && j == 20)
                qDebug() << c1 << " " << c2 << " " << c3 << " " << th;
            if (c1 > th && c2 > th && c3 > th) {
                for (int k=-N; k<=N; k++) {
                    for (int l=-N; l<=N; l++) {
                        if (k + i < 0 || k + i >= in.width() || l+j < 0 || l + j >= in.height()) {
                            continue;
                        }
                        ar[count] = RED(in.pixel(i+k, j+l));
                        ag[count] = GREEN(in.pixel(i+k, j+l));
                        ab[count] = BLUE(in.pixel(i+k, j+l));
                        count++;
                    }
                }
                qsort(ar, count, sizeof(int), compare_int_val);
                qsort(ag, count, sizeof(int), compare_int_val);
                qsort(ab, count, sizeof(int), compare_int_val);
                if (count % 2) {
                    r = PIXELRANGE(ROUND(ar[count / 2]));
                    g = PIXELRANGE(ROUND(ag[count / 2]));
                    b = PIXELRANGE(ROUND(ab[count / 2]));
                }
                else {
                    r = PIXELRANGE(ROUND( (ar[count / 2] + ar[count / 2 - 1]) / 2 ));
                    g = PIXELRANGE(ROUND( (ag[count / 2] + ag[count / 2 - 1]) / 2 ));
                    b = PIXELRANGE(ROUND( (ab[count / 2] + ab[count / 2 - 1]) / 2 ));
                }
                out.setPixel(i, j, PIXEL(PIXELRANGE(ROUND(r)), PIXELRANGE(ROUND(g)), PIXELRANGE(ROUND(b))));
            }
            else {
                out.setPixel(i, j, in.pixel(i, j));
            }
        }
    }
    delete [] ar;
    delete [] ag;
    delete [] ab;
}

void CTransform::AdMedFilter(QImage& in, QImage& out, int N, int delta, int th)
{
    int W = 2*(N+2) + 1;
    srand(QTime::currentTime().msec());
    int *ar = new int[W*W];
    int *ag = new int[W*W];
    int *ab = new int[W*W];
    int ttt = 0;
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            int c1 = RED(in.pixel(i, j));
            int c2 = GREEN(in.pixel(i, j));
            int c3 = BLUE(in.pixel(i, j));
            if (i == j && j == 20)
                qDebug() << "so: " << c1 << " " << c2 << " " << c3 << " " << th;
            if (c1 > th && c2 > th && c3 > th) {
                int M = N;
                bool flag = 1;
                int rmax = 0, gmax = 0, bmax = 0;
                int rmin = 300, gmin = 300, bmin = 300;
                int r = 0, g = 0, b = 0;
                for (M = N; M <= N + delta; M++) {
                    if (M == 2) ttt++;
                    int count = 0;
                    rmax = gmax = bmax = 0;
                    rmin = gmin = bmin = 300;
                    for (int k=-M; k<=M; k++) {
                        for (int l=-M; l<=M; l++) {
                            if (k + i < 0 || k + i >= in.width() || l+j < 0 || l + j >= in.height()) {
                                continue;
                            }
                            ar[count] = RED(in.pixel(i+k, j+l));
                            ag[count] = GREEN(in.pixel(i+k, j+l));
                            ab[count] = BLUE(in.pixel(i+k, j+l));
                            if (rmax < RED(in.pixel(i+k, j+l))) {
                                rmax = RED(in.pixel(i+k, j+l));
                            }
                            if (rmin > RED(in.pixel(i+k, j+l))) {
                                rmin = RED(in.pixel(i+k, j+l));
                            }
                            if (gmax < GREEN(in.pixel(i+k, j+l))) {
                                gmax = GREEN(in.pixel(i+k, j+l));
                            }
                            if (gmin > GREEN(in.pixel(i+k, j+l))) {
                                gmin = GREEN(in.pixel(i+k, j+l));
                            }
                            if (bmax < BLUE(in.pixel(i+k, j+l))) {
                                bmax = BLUE(in.pixel(i+k, j+l));
                            }
                            if (bmin > BLUE(in.pixel(i+k, j+l))) {
                                bmin = BLUE(in.pixel(i+k, j+l));
                            }
                            count++;
                        }
                    }
                    qsort(ar, count, sizeof(int), compare_int_val);
                    qsort(ag, count, sizeof(int), compare_int_val);
                    qsort(ab, count, sizeof(int), compare_int_val);
                    if (count % 2) {
                        r = PIXELRANGE(ROUND(ar[count / 2]));
                        g = PIXELRANGE(ROUND(ag[count / 2]));
                        b = PIXELRANGE(ROUND(ab[count / 2]));
                    }
                    else {
                        r = PIXELRANGE(ROUND( (ar[count / 2] + ar[count / 2 - 1]) / 2 ));
                        g = PIXELRANGE(ROUND( (ag[count / 2] + ag[count / 2 - 1]) / 2 ));
                        b = PIXELRANGE(ROUND( (ab[count / 2] + ab[count / 2 - 1]) / 2 ));
                    }
                    if (r - rmin > 0 && r - rmax < 0 && g - gmin > 0 && g - gmax < 0 && b - bmin > 0 && b - bmax < 0) {
                        flag = 0;
                        break;
                    }
                }
                if (flag) {
                    out.setPixel(i, j, in.pixel(i, j));
                }
                else {
                    if (RED(in.pixel(i, j)) - rmin > 0 && RED(in.pixel(i, j)) - rmax < 0 &&
                            GREEN(in.pixel(i, j)) - gmin > 0 && GREEN(in.pixel(i, j)) - gmax < 0 &&
                            BLUE(in.pixel(i, j)) - bmin > 0 && BLUE(in.pixel(i, j)) - bmax < 0) {
                        out.setPixel(i, j, PIXEL(PIXELRANGE(ROUND(r)), PIXELRANGE(ROUND(g)), PIXELRANGE(ROUND(b))));
                    }
                    else {
                        out.setPixel(i, j, in.pixel(i, j));
                    }
                }

                out.setPixel(i, j, PIXEL(PIXELRANGE(ROUND(r)), PIXELRANGE(ROUND(g)), PIXELRANGE(ROUND(b))));
            }
            else {
                out.setPixel(i, j, in.pixel(i, j));
            }
        }
    }
    qDebug() << ttt;
    delete [] ar;
    delete [] ag;
    delete [] ab;
}

void CTransform::MPFilter(QImage& in, QImage& out, int N)
{
    srand(QTime::currentTime().msec());
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            int count = 0;
            int rmax = 0, gmax = 0, bmax = 0;
            int rmin = 300, gmin = 300, bmin = 300;
            for (int k=-N; k<=N; k++) {
                for (int l=-N; l<=N; l++) {
                    if (k + i < 0 || k + i >= in.width() || l+j < 0 || l + j >= in.height()) {
                        continue;
                    }
                    count++;
                    //if (i==1) {qDebug() << bmin << " " << BLUE(in.pixel(i+k, j+l));}
                    if (rmax < RED(in.pixel(i+k, j+l))) {
                        rmax = RED(in.pixel(i+k, j+l));
                    }
                    if (rmin > RED(in.pixel(i+k, j+l))) {
                        rmin = RED(in.pixel(i+k, j+l));
                    }
                    if (gmax < GREEN(in.pixel(i+k, j+l))) {
                        gmax = GREEN(in.pixel(i+k, j+l));
                    }
                    if (gmin > GREEN(in.pixel(i+k, j+l))) {
                        gmin = GREEN(in.pixel(i+k, j+l));
                    }
                    if (bmax < BLUE(in.pixel(i+k, j+l))) {
                        bmax = BLUE(in.pixel(i+k, j+l));
                    }
                    if (bmin > BLUE(in.pixel(i+k, j+l))) {
                        bmin = BLUE(in.pixel(i+k, j+l));
                    }
                }
            }
            int p = PIXEL((rmax + rmin) / 2, (gmax + gmin) / 2, (bmax + bmin) / 2);

            //if (i == 1) {qDebug() << RED(in.pixel(i, j));}
            out.setPixel(i, j, p);
        }
    }
}

void CTransform::Sharpen(QImage& in, QImage& out, double strength)
{
    int N = 1;
    srand(QTime::currentTime().msec());
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            int count = 0;
            double r = 0, g = 0, b = 0;
            for (int k=-N; k<=N; k++) {
                for (int l=-N; l<=N; l++) {
                    if (k + i < 0 || k + i >= in.width() || l+j < 0 || l + j >= in.height()) {
                        continue;
                    }
                    count++;
                    r -= strength*RED(in.pixel(i+k, j+l));
                    g -= strength*GREEN(in.pixel(i+k, j+l));
                    b -= strength*BLUE(in.pixel(i+k, j+l));
                }
            }
            /*for (int l=-N; l<=N; l++) {
                if (l + i < 0 || l + i >= in.width() || l+j < 0 || l + j >= in.height()) {
                    continue;
                }
                count++;
                r -= RED(in.pixel(i, j+l));
                g -= GREEN(in.pixel(i, j+l));
                b -= BLUE(in.pixel(i, j+l));
                r -= RED(in.pixel(i+l, j));
                g -= GREEN(in.pixel(i+l, j));
                b -= BLUE(in.pixel(i+l, j));
            }*/
            r += (9*strength + 1)*(RED(in.pixel(i, j)));
            g += (9*strength + 1)*(GREEN(in.pixel(i, j)));
            b += (9*strength + 1)*(BLUE(in.pixel(i, j)));
            int p = PIXEL(PIXELRANGE(ROUND(r)), PIXELRANGE(ROUND(g)), PIXELRANGE(ROUND(b)));
            out.setPixel(i, j, p);
        }
    }
}

void spectrum(QImage& img, fftw_complex* data)
{
    double max = 0;
    for (int i=0; i<img.width(); i++) {
        for (int j=0; j<img.height(); j++) {
            double v = log(1 + sqrt(POW2(data[i*img.height()+j][0]) + POW2(data[i*img.height()+j][1])));
            if (v > max) {
                max = v;
            }
        }
    }

    for (int i=0; i<img.width(); i++) {
        for (int j=0; j<img.height(); j++) {
            double v = log(1 + sqrt(POW2(data[i*img.height()+j][0]) + POW2(data[i*img.height()+j][1])));
            int val = PIXELRANGE(ROUND( v / max * 255 ));
            img.setPixel(i, j, PIXEL(val, val, val));
        }
    }
}

void CTransform::FFT(QImage& in, QImage& out, int mode)
{
    double y;
    int cb, cr;
    fftw_complex* in_ = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * in.height()*in.width());
    //qDebug() << "FFT";
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
            in_[i*in.height()+j][0] = ((i+j)%2)?y:-y;
            //double r = BLUE(in.pixel(i, j));
            //in_[i*in.height()+j][0] = ((i+j)%2)?r:-r;
            in_[i*in.height()+j][1] = 0.0;
        }
    }
    fftw_plan p, p2;
    p = fftw_plan_dft_2d(in.height(), in.width(), in_, in_, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);


    int odd = 0;
    for (int i=0; i<50; i++) {
            in_[in.height()* (rand() % 512) + rand() % 512][0] = 1500000;
    }

    /*for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            double D = sqrt(POW2(i - (in.width()-1)/2) + POW2(j - (in.height()-1)/2));
            double k = (1 - exp(-0.05 * POW2((POW2(D)-POW2(78))/(D*2))));
            //MULC2R(in_[i*in.height()+j], k);
        }
    }*/
    //in_[200*in.height()+200][0] = 0;
    //in_[200*in.height()+300][0] = 0;

    p2 = fftw_plan_dft_2d(in.height(), in.width(), in_, in_, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(p2);

    if (mode == 1) {
        double max = 0;
        for (int i=0; i<in.width(); i++) {
            for (int j=0; j<in.height(); j++) {
                double v = fabs(in_[i*in.height()+j][0]);
                if (v > max) {
                    max = v;
                }
            }
        }

        for (int i=0; i<in.width(); i++) {
            for (int j=0; j<in.height(); j++) {
                RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
                int val = PIXELRANGE(ROUND(fabs(in_[i*in.height()+j][0]) / max * 255));
                out.setPixel(i, j, PIXEL(val, val, val));
                //double val = fabs(in_[i*in.height()+j][0]) / max * 255;
                //int intval = PIXELRANGE(ROUND(val));
                //out.setPixel(i, j, YCbCrtoRGB(val, cr, cb));
            }
        }
        for (int i=0; i<in.width(); i++) {
            for (int j=0; j<in.height(); j++) {
                //RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
                //in_[i*in.height()+j][0] = ((i+j)%2)?y:-y;
                double r = BLUE(out.pixel(i, j));
                in_[i*in.height()+j][0] = ((i+j)%2)?r:-r;
                in_[i*in.height()+j][1] = 0.0;
            }
        }
        fftw_execute(p);

        double vmin=fabs(in_[0][0]), vmax = fabs(in_[0][0]);
        for (int i=0; i<in.width(); i++) {
            for (int j=0; j<in.height(); j++) {
                if (fabs(in_[i*in.height()+j][0]) < vmin) {
                    vmin = fabs(in_[i*in.height()+j][0]);
                }
                if (fabs(in_[i*in.height()+j][0]) > vmax && POW2(fabs(in.height()/2-i))+POW2(fabs(in.height()/2-j)) > 25) {
                    vmax = fabs(in_[i*in.height()+j][0]);
                }
               /* if (in_[i*in.height()+j][0] > 1000000 && fabs(in.height()/2-i) > 500 && fabs(in.height()/2-j) > 500) {
                    in_[i*in.height()+j][0] = 0;
                    in_[i*in.height()+j][1] = 0;
                }*/
            }
        }
        int count = 0;
        for (int i=0; i<in.width(); i++) {
            for (int j=0; j<in.height(); j++) {
                if (fabs(in_[i*in.height()+j][0]) / vmax > 0.7 && POW2(fabs(in.height()/2-i))+POW2(fabs(in.height()/2-j)) > 100) {
                    count++;
                    double sum = 0, sum2 = 0, cnt = 0;
                    for (int k=-1; k<=1; k++) {
                        for (int l=-1; l<=1; l++) {
                            if (k + i < 0 || k + i >= in.width() || l+j < 0 || l + j >= in.height() || (!k && !l)) {
                                continue;
                            }
                            cnt++;
                            sum += in_[(i+k)*in.height()+(j+l)][0];
                            sum2 += in_[(i+k)*in.height()+(j+l)][0];
                        }
                    }
                    in_[i*in.height()+j][0] = sum / cnt;
                    in_[i*in.height()+j][1] = sum2 / cnt;
                }
            }
        }
        qDebug() << "X: " << vmin << "-" << vmax << " " << count;

        //spectrum(out, in_);
        //return;

        p2 = fftw_plan_dft_2d(in.height(), in.width(), in_, in_, FFTW_BACKWARD, FFTW_ESTIMATE);
        fftw_execute(p2);
    }

    double max = 0;
    double min = fabs(in_[0][0]);
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            double v = fabs(in_[i*in.height()+j][0]);
            if (v > max) {
                max = v;
            }
            if (v < min) {
                min = v;
            }
        }
    }

    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
            //int val = PIXELRANGE(ROUND(fabs(in_[i*in.height()+j][0]) / max * 255));
            //int val = BLUE(in.pixel(i, j));
            //out.setPixel(i, j, PIXEL(RED(in.pixel(i, j)), GREEN(in.pixel(i, j)), val));
            double val = (fabs(in_[i*in.height()+j][0])-min) / (max-min) * 255;
            int intval = PIXELRANGE(ROUND(val));
            out.setPixel(i, j, YCbCrtoRGB(val, cr, cb));
            //out.setPixel(i, j, PIXEL(intval, intval, intval));
        }
    }

    fftw_destroy_plan(p);
    fftw_destroy_plan(p2);
    fftw_free(in_);
}


void CTransform::HistEq(QImage& in, QImage& out)
{
    double Hvr[256], Hvg[256], Hvb[256];
    for (int i=0; i<256; i++) {
        Hvr[i] = Hvg[i] = Hvb[i] = 0;
    }
    int n = in.width()*in.height();
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            Hvr[RED(in.pixel(i, j))]++;
            Hvg[GREEN(in.pixel(i, j))]++;
            Hvb[BLUE(in.pixel(i, j))]++;
        }
    }
    Hvr[0] /= n;
    Hvg[0] /= n;
    Hvb[0] /= n;
    for (int i=1; i<256; i++) {
        Hvr[i] = Hvr[i] / n + Hvr[i-1];
        Hvg[i] = Hvg[i] / n + Hvg[i-1];
        Hvb[i] = Hvb[i] / n + Hvb[i-1];
    }
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            int r = RED(in.pixel(i, j));
            int g = GREEN(in.pixel(i, j));
            int b = BLUE(in.pixel(i, j));
            int p = PIXEL(PIXELRANGE(ROUND(Hvr[r]*255)), PIXELRANGE(ROUND(Hvr[g]*255)), PIXELRANGE(ROUND(Hvr[b]*255)));
            out.setPixel(i, j, p);
        }
    }
}

void CTransform::addFINoise(QImage& in, QImage& out, int num, double value, int ch)
{
    double y, r;
    int cb, cr;
    fftw_complex* in_ = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * in.height()*in.width());
    //qDebug() << "FFT";
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            switch(ch) {
            case channels::BLUE:
                r = BLUE(in.pixel(i, j));
                in_[i*in.height()+j][0] = ((i+j)%2)?r:-r;
                break;
            case channels::RED:
                r = RED(in.pixel(i, j));
                in_[i*in.height()+j][0] = ((i+j)%2)?r:-r;
                break;
            case channels::GREEN:
                r = GREEN(in.pixel(i, j));
                in_[i*in.height()+j][0] = ((i+j)%2)?r:-r;
                break;
            case channels::Y:
                RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
                in_[i*in.height()+j][0] = ((i+j)%2)?y:-y;
                break;
            }
            in_[i*in.height()+j][1] = 0.0;
        }
    }
    fftw_plan p, p2;
    p = fftw_plan_dft_2d(in.height(), in.width(), in_, in_, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    for (int i=0; i<num; i++) {
            in_[in.height()* (rand() % 512) + rand() % 512][0] = value;
    }
    spectrum(out, in_);
            return;
    p2 = fftw_plan_dft_2d(in.height(), in.width(), in_, in_, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(p2);

    double max = 0;
    double min = fabs(in_[0][0]);
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            double v = fabs(in_[i*in.height()+j][0]);
            if (v > max) {
                max = v;
            }
            if (v < min) {
                min = v;
            }
        }
    }

    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            double val = (fabs(in_[i*in.height()+j][0])-min) / (max-min) * 255;
            int intval = PIXELRANGE(ROUND(val));
            switch(ch) {
            case channels::BLUE:
                out.setPixel(i, j, PIXEL(RED(in.pixel(i, j)), GREEN(in.pixel(i, j)), intval));
                break;
            case channels::RED:
                out.setPixel(i, j, PIXEL(intval, GREEN(in.pixel(i, j)), BLUE(in.pixel(i, j))));
                break;
            case channels::GREEN:
                out.setPixel(i, j, PIXEL(RED(in.pixel(i, j)), intval, BLUE(in.pixel(i, j))));
                break;
            case channels::Y:
                RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
                out.setPixel(i, j, YCbCrtoRGB(val, cr, cb));
                break;
            }
        }
    }

    fftw_destroy_plan(p);
    fftw_destroy_plan(p2);
    fftw_free(in_);
}



void CTransform::Wiener(QImage& in, QImage& out, int N, double S, int radius, int stage)
{
    int ch = 3;
    double y, r;
    int cb, cr;

    qDebug() << "OOO" << stage;

    fftw_complex* in_ = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * in.height()*in.width());
    //qDebug() << "FFT";
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            switch(ch) {
            case channels::BLUE:
                r = BLUE(in.pixel(i, j));
                in_[i*in.height()+j][0] = ((i+j)%2)?r:-r;
                break;
            case channels::RED:
                r = RED(in.pixel(i, j));
                in_[i*in.height()+j][0] = ((i+j)%2)?r:-r;
                break;
            case channels::GREEN:
                r = GREEN(in.pixel(i, j));
                in_[i*in.height()+j][0] = ((i+j)%2)?r:-r;
                break;
            case channels::Y:
                RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
                in_[i*in.height()+j][0] = ((i+j)%2)?y:-y;
                break;
            }
            in_[i*in.height()+j][1] = 0.0;
        }
    }
    fftw_plan p, p2;
    p = fftw_plan_dft_2d(in.height(), in.width(), in_, in_, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    if (N > 0) {
        for (int i=0; i<in.width(); i++) {
            for (int j=0; j<in.height(); j++) {
                double D = sqrt(POW2(i - (in.width()-1)/2) + POW2(j - (in.height()-1)/2));
                if (D < radius) {
                    double k = 1 / (exp(-0.5*pow(D/S, 2)));
                    double h = 1 / (1 +k*k/N);
                    MULC2R(in_[i*in.height()+j], h*k);
                }
            }
        }
    }

    /*if (N > 0) {
        for (int i=0; i<in.width(); i++) {
            for (int j=0; j<in.height(); j++) {
                double D = sqrt(POW2(i - (in.width()-1)/2) + POW2(j - (in.height()-1)/2));
                double k = 1 / (exp(-0.5*pow(D/70, 2)));
                double k2 = exp(-0.5*pow(D/70, 2));
                k2 = k2*k2;
                MULC2R(in_[i*in.height()+j], k * k2 / (k2 + N));
            }
        }
    }*/

    if (stage == 4) {
        spectrum(out, in_);
        return;
    }

    /*spectrum(out, in_);
            return;*/

    if (stage == 4) {
        spectrum(out, in_);
        return;
    }

    p2 = fftw_plan_dft_2d(in.height(), in.width(), in_, in_, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(p2);

  /*  max = 0;
    min = fabs(in_[0][0]);
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            double v = fabs(in_[i*in.height()+j][0]);
            if (v > max) {
                max = v;
            }
            if (v < min) {
                min = v;
            }
        }
    }

    qDebug() << min / (in.height()*in.width()) << "_" << max / (in.height()*in.width());
*/
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            double val = (fabs(in_[i*in.height()+j][0])) / (in.height()*in.width());//((fabs(in_[i*in.height()+j][0])) - min) * (255.0 / (max-min));//(fabs(in_[i*in.height()+j][0])-min) / (max-min) * 255;//(fabs(in_[i*in.height()+j][0])) / (in.height()*in.width());
            int intval = PIXELRANGE(ROUND(val));
            switch(ch) {
            case channels::BLUE:
                out.setPixel(i, j, PIXEL(RED(in.pixel(i, j)), GREEN(in.pixel(i, j)), intval));
                break;
            case channels::RED:
                out.setPixel(i, j, PIXEL(intval, GREEN(in.pixel(i, j)), BLUE(in.pixel(i, j))));
                break;
            case channels::GREEN:
                out.setPixel(i, j, PIXEL(RED(in.pixel(i, j)), intval, BLUE(in.pixel(i, j))));
                break;
            case channels::Y:
                RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
                out.setPixel(i, j, YCbCrtoRGB(val, cr, cb));
                break;
            }
        }
    }

    fftw_destroy_plan(p);
    fftw_destroy_plan(p2);
    fftw_free(in_);
}

void CTransform::GaussBlur(QImage& in, QImage& out, double S, int stage)
{
    int ch = 3;
    double y, r;
    int cb, cr;

    //qDebug() << "OOO" << stage;

    fftw_complex* in_ = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * in.height()*in.width());
    //qDebug() << "FFT";
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            switch(ch) {
            case channels::BLUE:
                r = BLUE(in.pixel(i, j));
                in_[i*in.height()+j][0] = ((i+j)%2)?r:-r;
                break;
            case channels::RED:
                r = RED(in.pixel(i, j));
                in_[i*in.height()+j][0] = ((i+j)%2)?r:-r;
                break;
            case channels::GREEN:
                r = GREEN(in.pixel(i, j));
                in_[i*in.height()+j][0] = ((i+j)%2)?r:-r;
                break;
            case channels::Y:
                RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
                in_[i*in.height()+j][0] = ((i+j)%2)?y:-y;
                break;
            }
            in_[i*in.height()+j][1] = 0.0;
        }
    }
    fftw_plan p, p2;
    p = fftw_plan_dft_2d(in.height(), in.width(), in_, in_, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    if (stage == 0) {
        spectrum(out, in_);
        return;
    }

    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            double D = sqrt(POW2(i - (in.width()-1)/2) + POW2(j - (in.height()-1)/2));
            double k = (exp(-0.5*pow(D/S, 2)));
            MULC2R(in_[i*in.height()+j], k);
        }
    }

    if (stage == 7) {
        spectrum(out, in_);
        return;
    }

    /*for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            double D = sqrt(POW2(i - (in.width()-1)/2) + POW2(j - (in.height()-1)/2));
            double k = (exp(-0.5*pow(D/70, 2)));
            MULC2R(in_[i*in.height()+j], k);
        }
    }*/

    if (stage == 1) {
        spectrum(out, in_);
        return;
    }

    p2 = fftw_plan_dft_2d(in.height(), in.width(), in_, in_, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(p2);

    double max = 0;
    double min = fabs(in_[0][0]);
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            double v = fabs(in_[i*in.height()+j][0]);
            if (v > max) {
                max = v;
            }
            if (v < min) {
                min = v;
            }
        }
    }


    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            double val = (fabs(in_[i*in.height()+j][0])) / (in.height()*in.width());
            int intval = PIXELRANGE(ROUND(val));
            switch(ch) {
            case channels::BLUE:
                out.setPixel(i, j, PIXEL(RED(in.pixel(i, j)), GREEN(in.pixel(i, j)), intval));
                break;
            case channels::RED:
                out.setPixel(i, j, PIXEL(intval, GREEN(in.pixel(i, j)), BLUE(in.pixel(i, j))));
                break;
            case channels::GREEN:
                out.setPixel(i, j, PIXEL(RED(in.pixel(i, j)), intval, BLUE(in.pixel(i, j))));
                break;
            case channels::Y:
                RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
                out.setPixel(i, j, YCbCrtoRGB(val, cr, cb));
                break;
            }
        }
    }
    fftw_destroy_plan(p);
    fftw_destroy_plan(p2);
    fftw_free(in_);
}


void CTransform::Wavelet(QImage& in, QImage& out, int level)
{
    HMFilter(in, out, level);
    /*if (!level) {
        out = in;
        return;
    }
    vector<vector<double> > in_(in.height(), vector<double>(in.width()));
    vector<vector<double> > out_(in.height(), vector<double>(in.width()));
    double y;
    int cb, cr;
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
            in_[i][j] = y;
        }
    }
    string nm = "db2";
    vector<int> length;
    vector<double> output, flag;
    int J =1;
    dwt_2d(in_,J,nm,output,flag,length);
    for (int i=0; i<256; i++) {
        for (int j=0; j<256; j++) {
            output[(256+i)*512+j+256] += rand() % level;
            //output[(256+i)*512+j+256] = 0;
        }
    }
    idwt_2d(output,flag,nm,out_,length);


    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
            double val = PIXELRANGE(out_[i][j]);
            out.setPixel(i, j, YCbCrtoRGB(val, cr, cb));
            //int val = PIXELRANGE(ROUND(out_[i][j]));
            //out.setPixel(i, j, PIXEL(val, val, val));
        }
    }
    return;
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            RGBtoYCbCr(out.pixel(i, j), y, cr, cb);
            in_[i][j] = y;
        }
    }
    dwt_2d(in_,J,nm,output,flag,length);
    for (int i=0; i<256; i++) {
        for (int j=0; j<256; j++) {
            output[(256+i)*512+j+256] = 0;
            //output[(256+i)*512+j+256] = 0;
        }
    }
    idwt_2d(output,flag,nm,out_,length);
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            RGBtoYCbCr(in.pixel(i, j), y, cr, cb);
            double val = PIXELRANGE(out_[i][j]);
            out.setPixel(i, j, YCbCrtoRGB(val, cr, cb));
            //int val = PIXELRANGE(ROUND(out_[i][j]));
            //out.setPixel(i, j, PIXEL(val, val, val));
        }
    }
    return;

    for (int i=0; i<length.size(); i++) {
        qDebug() << "l: " << length[i];
    }
    qDebug() << output.size();
    int sz = 8;
    int sx = 0, sy = 0;
    int dx=0, dy=0;

    for (int i=0; i<sz; i++) {
        for (int j=0; j<sz; j++) {
            int val = PIXELRANGE(ROUND(output[i*sz+j]));
            out.setPixel(i, j, PIXEL(val, val, val));
        }
    }

    while (sz < 512) {
        for (int k=1; k<4; k++) {
            sx = (k%2)*sz;
            sy = (k/2)*sz;
            for (int i=0; i<sz; i++) {
                for (int j=0; j<sz; j++) {
                    int val = PIXELRANGE(ROUND(output[dx*dy+sz*sz*k+i*sz+j]));
                    out.setPixel(sx+i, sy+j, PIXEL(val, val, val));
                }
            }
        }
        sz *= 2;
        dx = sz;
        dy = sz;
    }*/
}
