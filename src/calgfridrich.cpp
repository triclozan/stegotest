#include "calgfridrich.h"
#include "dct.h"
#include <QTime>
#include <math.h>

CAlgFridrich::CAlgFridrich() {
    a = 0.1;
    gamma = 1;
    mode = 3;
}

void CAlgFridrich::Hide(QImage& container, QByteArray& watermark, QByteArray& key)
{
    qDebug() << a << " " << gamma;
    keyStruct keyData = *(keyStruct*)(key.constData());
    //qDebug() << keyData.seed;
    qsrand(keyData.seed);
    QBitArray bits = byteToBit(watermark);
    int bsize = bits.size();
    int k = 0;
    int l = 0;
    int m = 0;
    int X = container.width();
    int Y = container.height();
    int Nbx = 2;
    int Nby = 2;
    int Nb = Nbx*Nby;
    int Xb = X / Nbx;
    //double gamma = 1;
    int Yb = Y / Nby;
    int L = 20;
    double** A = new double* [Yb];
    double** B = new double* [Yb];
    int Nmid = Xb*Yb*0.3;//4080;
    double** Spr = new double* [Nb];
    for (int i=0; i<Nb; i++) {
        Spr[i] = new double [Nmid];
    }

    double** eps = new double* [bsize];
    double* eta = new double[bsize];
    int* start = new int[bsize];
    for (int i=0; i<bsize; i++) {
        eps[i] = new double [Nmid + 16];
        for (int j=0; j<Nmid+16; j++) {
            eps[i][j] = qrand() / (double)RAND_MAX;
        }
        start[i] = rand() % 6;
        if (bits[i]) {
            start[i] += 9;
        }
    }
    for (int b=0; b<Nb; b++) {
        for (int j=0; j<Nmid; j++) {
            double sum = 0;
            for (int i=0; i<bsize/Nb; i++) {
                sum += eps[b * bsize / Nb + i][j + start[b * bsize / Nb + i]];
            }
            Spr[b][j] = (sum - (bsize / Nb) / 2.0) / sqrt((bsize / Nb) / 12.0);
            //qDebug() << Spr[b][j];
        }
    }

    int** Cb = new int* [Yb];
    int** Cr = new int* [Yb];
    double cnorm;
    for (int j=0; j<Yb; j++) {
        A[j] = new double[Xb];
        B[j] = new double[Xb];
        Cb[j] = new int[Xb];
        Cr[j] = new int[Xb];
    }
    double y, cb, cr;
    double mean = 0;
    double stddev = 0;
    DCT Dct;
    for (int b=0; b<Nb; b++) {
        for (int i=0; i<Nmid; i++) {
            mean += Spr[b][i];
            stddev += Spr[b][i]*Spr[b][i];
        }
        mean /= (Nmid);
        stddev /= (Nmid);
        stddev -= mean*mean;
        stddev = sqrt(stddev);
        qDebug() << "stats:" << mean << " " << stddev;
    }
    //return;

    //double a = 0.3;
    double a2 = (1 + a) / (1 - a);
    double tau[3000];
    tau[0] = 1;
    int tau_max = 1;
    for (int i=1; i<3000; i++) {
        tau_max++;
        tau[i] = a2 * tau[i-1];
        if (tau[i] > 1000) {
            break;
        }
        //qDebug() << tau[i];
    }
    qDebug() << tau_max;
    int H = L;
    int count = L + 1;
    while (count < bits.size()) {
        H++;
        count += H + 1;
    }
    int Lm = Xb;
    int Hm = Lm;
    int j = 1;
    int countm = Xb;
    while (countm < Nmid) {
        Hm++;
        Lm--;
        countm += 2*(Xb-j);
        j++;
    }

    for (int b=0; b<Nb; b++) {
        int by = Yb * (b / Nbx);
        int bx = Xb * (b % Nbx);

        for (int i=0; i<Xb; i++) {
            for (int j=0; j<Yb; j++) {
                double v = container.pixel(i + bx, j + by) & 0xFF;
                mean += v;
                stddev += v*v;
            }
        }
        mean /= (Xb*Yb);
        stddev /= (Xb*Yb);
        stddev -= mean*mean;
        stddev = sqrt(stddev);
        cnorm = 1024 / (Xb*stddev);
        for (int i=0; i<Xb; i++) {
            for (int j=0; j<Yb; j++) {
                QColor color = QColor(container.pixel(bx+i, by+j));
                double Y = 0.299*color.red() + 0.587*color.green() + 0.114*color.blue();
                Cb[i][j] = -0.169*color.red() - 0.331*color.green() + 0.499*color.blue();
                Cr[i][j] = 0.499*color.red() - 0.418*color.green() - 0.0813*color.blue();
                A[i][j] = (Y - mean) * cnorm;
            }
        }

        Dct.fdct(A, B, Xb, Yb);



        count = 0;
        countm = 0;
        double max_coeff = 0;
        for (int i1=0; i1<Xb; i1++) {
            for (int i2=0; i2<Yb; i2++) {
                if (L <= i1 + i2 && i1 + i2 <= H) {
                    if (B[i1][i2] > max_coeff) {
                        max_coeff = B[i1][i2];
                    }
                    double d = fabs(B[i1][i2]);
                    int sign = SIGN(B[i1][i2]);
                    double t1, t2, t = 1;
                    int ind;
                    //qDebug() << B[i1][i2];
                    for (ind=0; ind<tau_max; ind++) {
                        if (d < tau[ind]) {
                            break;
                        }
                        t = -t;
                    }
                    if (SIGN(bits[count]) == t) {
                        if (ind == 0) {
                            t1 = 0;
                            t2 = tau[0];
                        }
                        else {
                            t1 = tau[ind - 1];
                            t2 = tau[ind];
                        }
                        if (fabs(d - t1) < 0.5 || fabs(d - t2) < 0.5) {
                            B[i1][i2] = sign * ((t2 - t1) * 0.5/*((qrand()%50 + 25) / 100.0)*/ + t1) /*/ 2.0*/;
                        }
                    }
                    else {
                        if (ind < 2) {
                            t1 = tau[ind];
                            t2 = tau[ind + 1];
                        }
                        else {
                            t1 = tau[ind - 2];
                            t2 = tau[ind - 1];
                        }
                        B[i1][i2] = sign * ((t2 - t1) * 0.5/*((qrand()%50 + 25) / 100.0)*/ + t1) /*/ 2.0*/;
                    }

                    count++;
                    if (count >= bits.size()) break;
                }
            }
            if (count >= bits.size()) break;
        }
        qDebug() << max_coeff;
        for (int i1=0; i1<Xb; i1++) {
            for (int i2=0; i2<Yb; i2++) {
                if (Lm <= i1 + i2 + 1 && i1 + i2 + 1 <= Hm) {
                    B[i1][i2] += Spr[b][countm] * gamma;
                    //qDebug () << B[i1][i2];
                    countm++;
                    if (countm >= Nmid) break;
                }
            }
            if (countm >= Nmid) break;
        }
        Dct.idct(B, A, Xb, Yb);

        for (int i1=0; i1<Xb; i1++) {
            for (int i2=0; i2<Yb; i2++) {
                //QColor color = QColor(container.pixel(bx+i1, by+i2));
                QColor color;
                double Y = A[i1][i2] / cnorm + mean;
                double R = Y + Cr[i1][i2] + (Cr[i1][i2] >> 2) + (Cr[i1][i2] >> 3) + (Cr[i1][i2] >> 5);
                double G = Y - ((Cb[i1][i2] >> 2) + (Cb[i1][i2] >> 4) + (Cb[i1][i2] >> 5)) - ((Cr[i1][i2] >> 1) + (Cr[i1][i2] >> 3) + (Cr[i1][i2] >> 4) + (Cr[i1][i2] >> 5));
                double B = Y + Cb[i1][i2] + (Cb[i1][i2] >> 1) + (Cb[i1][i2] >> 2) + (Cb[i1][i2] >> 6);
                color.setRed(PIXELRANGE(ROUND(R)));
                color.setGreen(PIXELRANGE(ROUND(G)));
                color.setBlue(PIXELRANGE(ROUND(B)));
                /*color.setRed(PIXELRANGE(ROUND(Y)));
                color.setGreen(PIXELRANGE(ROUND(Y)));
                color.setBlue(PIXELRANGE(ROUND(Y)));*/
                /*if (i < 10) {
                    //cout << PIXELRANGE(ROUND(A[i1][i2])) << "\n";
                }*/
                QRgb val = color.rgb();
                container.setPixel(bx+i1, by+i2, val);
            }
        }
    }

    for (int i=0; i<Yb; i++) {
        delete [] A[i];
        delete [] B[i];
    }
    for (int i=0; i<Nb; i++) {
        delete [] Spr[i];
    }
    for (int i=0; i<bsize; i++) {
        delete [] eps[i];
    }
    delete [] A;
    delete [] B;
    delete [] Spr;
    delete [] eps;
    delete [] eta;
    //container.save("F:\\koh.bmp");
}

void CAlgFridrich::Restore(QImage& container, QByteArray& watermark, QByteArray& key)
{
    keyStruct keyData = *(keyStruct*)(key.constData());
    qsrand(keyData.seed);
    //qDebug() << keyData.seed;
    int length = keyData.length;
    //cout << keyData.length;
    QBitArray bits;
    bits.resize(length);
    int bsize = bits.size();
    //double gamma = 1;
    int k = 0;
    int l = 0;
    int X = container.width();
    int Y = container.height();
    int Nbx = 2;
    int Nby = 2;
    int Nb = Nbx*Nby;
    int Xb = X / Nbx;
    int Yb = Y / Nby;
    int Nmid = Xb*Yb*0.3; //4080;
    int L = 20;
    double** A = new double* [Yb];
    double** B = new double* [Yb];
    double* data = new double[length];
    for (int i=0; i<length; i++) {
        data[i] = 0;
    }
    double cnorm;
    for (int j=0; j<Yb; j++) {
        A[j] = new double[Xb];
        B[j] = new double[Xb];
    }
    double y, cb, cr;
    double mean = 0;
    double stddev = 0;
    DCT Dct;
    double* mid = new double [Nmid];
    //double a = 0.3;
    double a2 = (1 + a) / (1 - a);
    double tau[3000];
    tau[0] = 1;
    int tau_max = 1;
    for (int i=1; i<3000; i++) {
        tau_max++;
        tau[i] = a2 * tau[i-1];
        if (tau[i] > 1000) {
            break;
        }
        //qDebug() << tau[i];
    }

    int H = L;
    int count = L + 1;
    while (count < bits.size()) {
        H++;
        count += H + 1;
    }

    int Lm = Xb;
    int Hm = Lm;
    int j = 1;
    int countm = Xb;
    while (countm < Nmid) {
        Hm++;
        Lm--;
        countm += 2*(Xb-j);
        j++;
    }

    double K[16];
    double** eps = new double* [bsize];
    double* eta = new double[bsize];
    int* start = new int[bsize];
    for (int i=0; i<bsize; i++) {
        eps[i] = new double [Nmid + 16];
        for (int j=0; j<Nmid+16; j++) {
            eps[i][j] = qrand() / (double)RAND_MAX;
        }
        rand();
    }
    //qDebug() << eps[100][100];
    for (int b=0; b<Nb; b++) {
        int by = Yb * (b / Nbx);
        int bx = Xb * (b % Nbx);

        for (int i=0; i<Xb; i++) {
            for (int j=0; j<Yb; j++) {
                double v = container.pixel(i + bx, j + by) & 0xFF;
                mean += v;
                stddev += v*v;
            }
        }
        mean /= (Xb*Yb);
        stddev /= (Xb*Yb);
        stddev -= mean*mean;
        stddev = sqrt(stddev);
        cnorm = 1024 / (Xb*stddev);
        for (int i=0; i<Xb; i++) {
            for (int j=0; j<Yb; j++) {
                QColor color = QColor(container.pixel(bx+i, by+j));
                double Y = 0.299*color.red() + 0.587*color.green() + 0.114*color.blue();
                A[i][j] = (Y - mean) * cnorm;
            }
        }

        Dct.fdct(A, B, Xb, Yb);

        count = 0;
        countm = 0;

        qDebug() << "mode " << mode;

        if (mode & 1) {
            for (int i1=0; i1<Xb; i1++) {
                for (int i2=0; i2<Yb; i2++) {
                    if (L <= i1 + i2 && i1 + i2 <= H) {
                        //qDebug() << B[i1][i2];
                        double d = fabs(B[i1][i2]);
                        int t = 1;
                        int ind;
                        for (ind=0; ind<tau_max; ind++) {
                            if (d < tau[ind]) {
                                break;
                            }
                            t = -t;
                        }
                        data[count] += t;
                        count++;
                        if (count >= bits.size()) break;
                    }
                }
                if (count >= bits.size()) break;
            }
        }

        if (mode & 2) {
            for (int i1=0; i1<Xb; i1++) {
                for (int i2=0; i2<Yb; i2++) {
                    if (Lm <= i1 + i2 + 1 && i1 + i2 + 1 <= Hm) {
                        mid[countm] = B[i1][i2];

                        countm++;
                        if (countm >= Nmid) break;
                    }
                }
                if (countm >= Nmid) break;
            }

            for (int i=0; i<bsize/Nb; i++) {
                int cf = b * bsize / Nb + i;
                int y = 0;
                for (int m=0; m<16; m++) {
                    K[m] = 0;
                    for (int j=0; j<Nmid; j++) {
                        if (mid[y] < mid[j]) {
                            y = j;
                        }
                        K[m] += mid[j] * eps[cf][j + m];
                    }
                }
                //qDebug() << mid[y];
                int ind_max = 0;
                for (int m=0; m<16; m++) {
                    if (K[m] > K[ind_max]) {
                        ind_max = m;
                    }
                }
                //qDebug() << mid[0] * eps[b * bsize / Nb + i][0];
                if (ind_max <= 7) {
                    data[b * bsize / Nb + i]--;
                }
                else {
                    data[b * bsize / Nb + i]++;
                }
            }
        }
    }

    for (int i=0; i<length; i++) {
        //qDebug() << data[i];
        if (data[i] > 0) {
            bits[i] = 1;
        }
        else {
            bits[i] = 0;
        }
    }
    watermark = bitToByte(bits);

    for (int i=0; i<Yb; i++) {
        delete [] A[i];
        delete [] B[i];
    }
    delete [] A;
    delete [] B;
    delete [] data;
    delete [] mid;
    for (int i=0; i<bsize; i++) {
        delete [] eps[i];
    }
    delete [] eps;
    delete [] eta;
}


void CAlgFridrich::GenKey(QByteArray& data)
{
    keyStruct keyData;
    keyData.seed = QTime::currentTime().msec();
    keyData.length = data.size()*8;

    key = QByteArray((char*)&keyData, sizeof(keyStruct));
}

void CAlgFridrich::SetParams(QString& params)
{
    CParamHelper ph;
    ph.AddToMap(&a, ph.DOUBLE, "alpha");
    ph.AddToMap(&gamma, ph.DOUBLE, "gamma");
    ph.AddToMap(&mode, ph.INT, "mode");
    ph.SetParams(params);
}
