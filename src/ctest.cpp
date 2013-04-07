#include "ctest.h"
#include <QImage>
#include <QTime>

#include <QDebug>
#include <QBuffer>
#include <math.h>
#include "cparamhelper.h"
#include "util.h"

void CTest::Test(CAlgorithm* alg, QString& params, QString& alg_params, QByteArray& result, const QString& container_name, const QString& watermark_name)
{
    QImage in;
    //QImage in2;
    QBuffer buf;
    in.load(container_name);
    QImage orig(in);
    QFile wm(watermark_name);
    wm.open(QIODevice::ReadOnly);
    QByteArray bytes;// = wm.readAll();
    QByteArray bytes_out;
    QBitArray bits, bits_out, tmp_bits_out;
    wm.close();
    /*if (!size) {
        size = (in.width() / 8) * (in.height() / 8) / 8;
        tsize = size;
    }
    else {
        if (!tsize) {
            tsize = (in.width() / 8) * (in.height() / 8) / 8;
        }
        rep = tsize / size;
        tsize = size*rep;
    }
*/
    srand(QTime::currentTime().msec());
    int count = 0;
    int bcount = 0;
    tsize = size = 0;
    num = 2;
    CTest::SetParams(params);
    if (!size) {
        size = (in.width() / 8) * (in.height() / 8) / 8;
    }
    if (!tsize) {
        tsize = size;
    }
    int rep = tsize / size;
    bytes.resize(size*rep);
    /*for (int i=0; i<size; i++) {
        bytes[i] = rand() % 256;
    }
    bytes = bytes.repeated(rep);*/
    SetParams(params);
    qDebug() << rep << size;
    for (int i=0; i<num; i++) {
        for (int i=0; i<size; i++) {
            bytes[i] = rand() % 256;// - bytes[i];
            for (int j=1; j<rep; j++) {
                bytes[i+size*j] = bytes[i];
            }
        }
        alg->SetParams(alg_params);
        //qDebug() << alg_params;
        alg->GenKey(bytes);
        alg->Hide(in, bytes);
        Attack(in, params);
        alg->Restore(in, bytes_out);
        //in.save("F:\\1.jpg");
        int pos = 0;
        for (int i=0; i< bytes.size(); i++) {
            if (bytes[i] == bytes_out[i]) {
                bcount++;
            }
            if (bytes_out[i] == 255) {
                pos++;
            }
        }
        qDebug() << pos;
        bits = alg->byteToBit(bytes);
        tmp_bits_out = alg->byteToBit(bytes_out);

        bits_out.resize(size*8);

        for (int i=0; i<size*8; i++) {
            int v = 0;
            for (int j=0; j<rep; j++) {
                if (tmp_bits_out[size*8*j+i]) {
                    v++;
                    pos++;
                }
                else {
                    v--;
                }
            }
            if (v > 0) {
                bits_out[i] = true;

            }
            else {
                bits_out[i] = false;
            }
        }
        //qDebug() << bytes_out.size();
        //qDebug() << bytes.size();

        for (int i=0; i< bits_out.size(); i++) {
            if (bits[i] == bits_out[i]) {
                count++;
            }
        }
        /*for (int i=0; i<tsize; i++) {
            bytes[i] = 255 ^ bytes[i];// - bytes[i];
        }*/

        in = orig;
        //qDebug() << "WWW";
    }
    sResults res;
    res.ByER = ((double)count) / bytes.size() / num;
    res.BER = ((double)count) / (size*8) / num;
    result = SERIALIZE(res);
}

void CTest::Visual(CAlgorithm* alg, QString& params, QString& alg_params, const QString& container_name, const QString* watermark_name, int wm_size)
{
    CParamHelper ph;
    QString res_name;
    ph.AddToMap(&res_name, ph.STRING, "name");
    ph.SetParams(params);
    QImage in;
    in.load(container_name);
    QImage orig(in);
    QByteArray bytes;
    if (watermark_name != 0) {
        QFile wm(*watermark_name);
        wm.open(QIODevice::ReadOnly);
        bytes = wm.readAll();
        wm.close();
    }
    else {
        srand(QTime::currentTime().msec());
        bytes.resize(wm_size);
        for (int i=0; i<wm_size; i++) {
            bytes[i] = rand() % 256;
        }
    }

    QByteArray bytes_out;
    //qDebug() << alg_params;
    alg->SetParams(alg_params);
    alg->GenKey(bytes);
    alg->Hide(in, bytes);
    alg->Restore(in, bytes_out);
    //qDebug() << res_name;
    in.save(res_name);
}

void CTest::SetParams(QString params)
{
    this->params = params;
    CParamHelper ph;
    ph.AddToMap(&size, ph.INT, "size");
    ph.AddToMap(&tsize, ph.INT, "tsize");
    ph.AddToMap(&num, ph.INT, "num");
    ph.SetParams(params);
}

void CTest::Attack(QImage& img, QString& params)
{

}


int compare_int (const void * a, const void * b)
{
  return ( *(int*)a - *(int*)b );
}



void CTest::addNoise(QImage& in, QImage& out, int level)
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

void CTest::addGNoise(QImage& in, QImage& out, int level)
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

void CTest::addINoise(QImage& in, QImage& out, int level)
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

void CTest::addBINoise(QImage& in, QImage& out, int level)
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

void CTest::AMFilter(QImage& in, QImage& out, int N)
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
            int p = PIXEL(r/count, g/count, b/count);
            out.setPixel(i, j, p);
        }
    }
}

void CTest::GMFilter(QImage& in, QImage& out, int N)
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


void CTest::HMFilter(QImage& in, QImage& out, int N)
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

void CTest::MedFilter(QImage& in, QImage& out, int N)
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
            qsort(ar, count, sizeof(int), compare_int);
            qsort(ag, count, sizeof(int), compare_int);
            qsort(ab, count, sizeof(int), compare_int);
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
    }
    delete [] ar;
    delete [] ag;
    delete [] ab;
}

void CTest::AdMedFilter(QImage& in, QImage& out, int N)
{
    int W = 2*(N+2) + 1;
    srand(QTime::currentTime().msec());
    int *ar = new int[W*W];
    int *ag = new int[W*W];
    int *ab = new int[W*W];
    for (int i=0; i<in.width(); i++) {
        for (int j=0; j<in.height(); j++) {
            int M = N;
            bool flag = 1;
            int rmax = 0, gmax = 0, bmax = 0;
            int rmin = 300, gmin = 300, bmin = 300;
            int r = 0, g = 0, b = 0;
            for (M = N; M <= N + 2; M++) {
                //if (M > N) qDebug() << M;
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
                qsort(ar, count, sizeof(int), compare_int);
                qsort(ag, count, sizeof(int), compare_int);
                qsort(ab, count, sizeof(int), compare_int);
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
    }
    delete [] ar;
    delete [] ag;
    delete [] ab;
}

void CTest::MPFilter(QImage& in, QImage& out, int N)
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

void CTest::Sharpen(QImage& in, QImage& out, double strength)
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

