#ifndef DCT_H
#define DCT_H
#define NJPEG 8

#include "util.h"

static const unsigned int quant[NJPEG][NJPEG] = {
    {16,  11,  10,  16,  24,  40,  51,  61},
    {12,  12,  14,  19,  26,  58,  60,  55},
    {14,  13,  16,  24,  40,  57,  69,  56},
    {14,  17,  22,  29,  51,  87,  80,  62},
    {18,  22,  37,  56,  68, 109, 103,  77},
    {24,  35,  55,  64,  81, 104, 113,  92},
    {49,  64,  78,  87, 103, 121, 120, 101},
    {72,  92,  95,  98, 112, 100, 103,  99}};



class DCT
{
public:
    DCT();
    void fdct(double input[8][8] , double output[8][8]);
    void idct(double input[8][8] , double output[8][8]);
    void fdct(double** input, double** output, int X, int Y);
    void idct(double** input, double** output, int X, int Y);
    void fdct2(double* input, double* output, int X, int Y);
    void idct2(double* input, double* output, int X, int Y);
    ~DCT();
    //static DCT Dct;
private:
    void init(int X, int Y);
    void free_cm();
    double C[8][8], Ct[8][8];
    double** C2, **Ct2;
    int X, Y;
};

//DCT DCT::Dct;

#endif // DCT_H
