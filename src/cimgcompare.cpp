#include "cimgcompare.h"
#include <QImage>
#include <QDebug>
#include "math.h"
#include "util.h"

CImgCompare::CImgCompare()
{
}

CImgCompare::Result CImgCompare::PSNR_B(QImage& orig, QImage& res, int channel)
{
    Result result;
    double mse = 0;
    double md = -1000000, mv = -1000000;
    double ad, nad, nmse, l2, lmse, snr, psnr, ifid, nc, cq, sc, hs = 0;
    double sd = 0, sd2 = 0, sv = 0, sv2 = 0, su2 = 0, sp = 0;
    int Hv[256], Hu[256];
    for (int i=0; i<256; i++) {
        Hv[i] = Hu[i] = 0;
    }
    int x = orig.width(), y = orig.height(), xy = x*y;
    int v, u;
    for (int i=0; i<x; i++) {
        for (int j=0; j<y; j++) {
            switch (channel) {
            case channels::BLUE:
                v = orig.pixel(i, j) & 0xFF;
                u = res.pixel(i, j) & 0xFF;
                break;
            case channels::GREEN:
                v = (orig.pixel(i, j) & 0xFF00) >> 8;
                u = (res.pixel(i, j) & 0xFF00) >> 8;
                break;
            case channels::RED:
                v = (orig.pixel(i, j) & 0xFF0000) >> 16;
                u = (res.pixel(i, j) & 0xFF0000) >> 16;
                break;
            case channels::Y:
                double vd = 0.299*((orig.pixel(i, j) & 0xFF0000) >> 16) + 0.587*((orig.pixel(i, j) & 0xFF00) >> 8) + 0.114*(orig.pixel(i, j) & 0xFF);
                double ud = 0.299*((res.pixel(i, j) & 0xFF0000) >> 16) + 0.587*((res.pixel(i, j) & 0xFF00) >> 8) + 0.114*(res.pixel(i, j) & 0xFF);
                v = PIXELRANGE(ROUND(vd));
                u = PIXELRANGE(ROUND(ud));
            }
            Hv[v]++;
            Hu[u]++;
            double d = fabs( v - u );
            double p = v*u;
            if (v > mv) {
                mv = v;
            }
            sp += p;
            sv += v;
            sv2 += v*v;
            su2 += u*u;
            sd += d;
            sd2 += d * d;
            if (d > md) {
                md = d;
            }
        }
    }
    for (int i=0; i<256; i++) {
        hs += abs(Hv[i] - Hu[i]);
    }
    ad = sd / xy;
    nad = sd / sv;
    mse = sd2 / xy;
    nmse = sd2 / sv2;
    l2 = sqrt(sd2 / xy);
    snr = 1 / nmse;
    psnr = mv * mv / mse;    //10 * log10(mv*mv/mse);
    ifid = 1 - nmse;
    nc = sp / sv2;
    cq = sp / sv;
    sc = sv2 / su2;
    hs = hs;

    result.md = md;
    result.ad = ad;
    result.nad = nad;
    result.mse = mse;
    result.nmse = nmse;
    result.l2 = l2;
    result.snr = snr;
    result.psnr = psnr;
    result.ifid = ifid;
    result.nc = nc;
    result.cq = cq;
    result.sc = sc;
    result.hs = hs;

    return result;
}


