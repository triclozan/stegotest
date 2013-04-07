#ifndef UTIL_H
#define UTIL_H

#define ROUND(a) (((a) < 0) ? (int) ((a) - 0.5) : (int) ((a) + 0.5))
#define PIXELRANGE(P) (((P) > 255) ? 255 : (((P) < 0) ? 0 : (P)))
#define SIGN(P) (((P) > 0) ? 1 : -1)
#define BLUE(P) ((P) & 0x000000FF)
#define GREEN(P) (((P) & 0x0000FF00) >> 8)
#define RED(P) (((P) & 0x00FF0000) >> 16)
#define SET_BLUE(P,V) (((P) & 0xFFFFFF00) | (V))
#define SET_GREEN(P,V) (((P) & 0xFFFF00FF) | ((V) << 8))
#define SET_RED(P,V) (((P) & 0xFF00FFFF) | ((V) << 16))
#define SERIALIZE(V) (QByteArray((char*)&(V), sizeof((V))))
#define DESERIALIZE(B,T) *((T*)((B).constData()))
#define PIXEL(R,G,B) (((R) << 16) | ((G) << 8) | (B))
#define POW2(a) ((a)*(a))
#define MULC2R(c,r) (c)[0] *= (r); (c)[1] *= (r)
#define MULC2C(c1,c2) (c1)[0] = (c1)[0] * (c2)[0] - (c1)[1] * (c2)[1]; (c1)[1] = (c1)[0] * (c2)[1] + (c1)[1] * (c2)[0];
//MULC2R

namespace channels {
    enum channels {BLUE=0, GREEN=1, RED=2, Y=3};
}

enum Transforms {tUNoise=1, tINoise=2, tBINoise=3, tGNoise=4, tFINoise=5, tAMFilter=10, tGMFilter=11, tHMFilter=12, tMedFilter=13, tAdMedFilter=14, tBLFilter=15, tAdLocalFilter=16, tMPFilter=17, tSharpen=20, tJpeg=30, tFFT=50, tWavelet=51, tWiener=52, tHisteq=60};

unsigned int YCbCrtoRGB(double y, int cr, int cb);
void RGBtoYCbCr(unsigned int value, double& y, int& cr, int& cb);
double RGBtoY(unsigned int value);
#endif // UTIL_H
