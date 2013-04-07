#include "dct.h"
#include <math.h>
#include <iostream>

#include "FFT/fftw3.h"
using namespace std;


DCT::DCT()
{
    for (int j = 0; j < NJPEG; j++ ) {
      C[0][j] = 1.0 / sqrt( (double) NJPEG );
      Ct[j][0] = C[0][j];
    }

    for (int i = 1 ; i < NJPEG ; i++ ) {
      for (int j = 0 ; j < NJPEG ; j++ ) {
        C[i][j] = sqrt( 2.0 / NJPEG ) * cos( M_PI * ( 2 * j + 1 ) * i / ( 2.0 * NJPEG ) );
        Ct[j][i] = C[i][j];
      }
    }
    C2 = Ct2 = 0;
    X = Y = -1;
}

void DCT::fdct(double input[8][8] , double output[8][8]) {
    double temp[NJPEG][NJPEG];
    double temp1;

    for (int i = 0 ; i < NJPEG ; i++ ) {
        for (int j = 0 ; j < NJPEG ; j++ ) {
            temp[i][j] = 0.0;
            for (int k = 0 ; k < NJPEG ; k++ ) {
                 temp[i][j] += ( input[ i ][ k ]/* - 128 */) * Ct[k][j];
            }
        }
    }

    for (int i = 0 ; i < NJPEG ; i++ ) {
        for (int j = 0 ; j < NJPEG ; j++ ) {
            temp1 = 0.0;
            for (int k = 0 ; k < NJPEG ; k++ ) {
                temp1 += C[ i ][ k ] * temp[k][j];
            }
            output[i][j] = temp1;
        }
    }
}

void DCT::idct(double input[8][8] , double output[8][8]) {
    double temp[NJPEG][NJPEG];
    double temp1;

    for (int i = 0 ; i < NJPEG ; i++ ) {
        for (int j = 0 ; j < NJPEG ; j++ ) {
            temp[i][j] = 0.0;
            for (int k = 0 ; k < NJPEG ; k++ ) {
                temp[i][j] += input[i][k] * C[k][j];
            }
        }
    }

    for (int i = 0 ; i < NJPEG ; i++ ) {
        for (int j = 0 ; j < NJPEG ; j++ ) {
            temp1 = 0.0;
            for (int k = 0 ; k < NJPEG ; k++ ) {
                temp1 += Ct[i][k] * temp[k][j];
            }
            output[i][j] = temp1/* + 128.0*/;
        }
    }
}

void DCT::fdct(double** input, double** output, int X, int Y)
{
    init(X, Y);

    double **temp = new double* [Y];
    for (int i=0; i<Y; i++) {
        temp[i] = new double[X];
    }
    double temp1;

    for (int i=0; i<X; i++) {
        for (int j=0; j<Y; j++) {
            temp[i][j] = 0.0;
            for (int k=0; k<X; k++) {
                 temp[i][j] += ( input[i][k] /*- 128*/ ) * Ct2[k][j];
            }
        }
    }

    for (int i=0; i<X; i++) {
        for (int j=0; j<Y; j++) {
            temp1 = 0.0;
            for (int k=0; k<X; k++) {
                temp1 += C2[i][k] * temp[k][j];
            }
            output[i][j] = temp1;
        }
    }


}

void DCT::fdct2(double* input, double* output, int X, int Y)
{
    fftw_plan p;
    p = fftw_plan_r2r_2d(X, Y, input, output, FFTW_REDFT10, FFTW_REDFT10, FFTW_ESTIMATE);
    fftw_execute(p);
    double m = 4 * sqrt(X/2) * sqrt(Y/2);
    double m1 = m * sqrt(2);
    double m2 = m * 2;
    for (int i=0; i<X; i++) {
        for (int j=0; j<Y; j++) {
            if (!i && !j) {
                output[i * Y + j] /= m2;
            }
            else if (!j || !i) {
                output[i * Y + j] /= m1;
            }
            else {
                output[i * Y + j] /= m;
            }
        }
    }
    fftw_destroy_plan(p);
}

void DCT::idct2(double* input, double* output, int X, int Y)
{
    double m = 4 * sqrt(X/2) * sqrt(Y/2);
    double m1 = m * sqrt(2);
    double m2 = m * 2;
    for (int i=0; i<X; i++) {
        for (int j=0; j<Y; j++) {
            if (!i && !j) {
                input[i * Y + j] *= m2;
            }
            else if (!j || !i) {
                input[i * Y + j] *= m1;
            }
            else {
                input[i * Y + j] *= m;
            }
        }
    }
    fftw_plan p;
    p = fftw_plan_r2r_2d(X, Y, input, output, FFTW_REDFT01, FFTW_REDFT01, FFTW_ESTIMATE);
    fftw_execute(p);
    for (int i=0; i<X; i++) {
        for (int j=0; j<Y; j++) {
            output[i * Y + j] /= (4 * X * Y);
        }
    }
    fftw_destroy_plan(p);
}

void DCT::idct(double** input, double** output, int X, int Y)
{
    init(X, Y);
    double **temp = new double* [Y];
    for (int i=0; i<Y; i++) {
        temp[i] = new double[X];
    }
    double temp1;

    for (int i = 0 ; i < X ; i++ ) {
        for (int j = 0 ; j < Y ; j++ ) {
            temp[i][j] = 0.0;
            for (int k = 0 ; k < X ; k++ ) {
                temp[i][j] += input[i][k] * C2[k][j];
            }
        }
    }

    for (int i=0; i<X ; i++) {
        for (int j=0; j<Y; j++) {
            temp1 = 0.0;
            for (int k=0; k<X; k++) {
                temp1 += Ct2[i][k] * temp[k][j];
            }
            output[i][j] = temp1 /*+ 128.0*/;
        }
    }
    /*for (int i=0; i<8; i++) {
        for (int j=0; j<8; j++) {
            cout << output[i][j] << " ";
        }
        cout << "\n";
    }*/
}

DCT::~DCT()
{
    free_cm();
}

void DCT::free_cm()
{
    if (C2 != 0) {
        for (int j=0; j<Y; j++) {
            delete [] C2[j];
        }
        delete [] C2;
    }
    if (Ct2 != 0) {
        for (int j=0; j<Y; j++) {
            delete [] Ct2[j];
        }
        delete [] Ct2;
    }
}

void DCT::init(int X, int Y)
{
    if (X == this->X && Y == this->Y) {
        return;
    }

    free_cm();

    this->X = X;
    this->Y = Y;
    C2 = new double* [Y];
    Ct2 = new double* [Y];
    for (int i=0; i<Y; i++) {
        C2[i] = new double[X];
        Ct2[i] = new double[X];
    }

    /*int i, group, base, item, nitems, halfN;
    double factor;

    dct_NxN_log2N = -1;
    do {
      dct_NxN_log2N++;
      if ((1<<dct_NxN_log2N)>N) {
        fprintf(stderr, "dct_NxN: %d not a power of 2\n", N);
        exit(1);
      }
    } while((1<<dct_NxN_log2N)<N);

    halfN=N/2;

    for (i=0; i<=halfN-1; i++) {
        dct_NxN_costable[halfN+i]=4*i+1;
    }

    for (group=1;group<=dct_NxN_log2N-1;group++) {
      base= 1<<(group-1);
      nitems=base;
      factor = 1.0*(1<<(dct_NxN_log2N-group));
      for (item=1; item<=nitems; item++) {
          dct_NxN_costable[base+item-1]=factor*dct_NxN_costable[halfN+item-1];
      }
    }

    for(i=1;i<=N-1;i++) dct_NxN_costable[i] = 1.0/(2.0*cos(dct_NxN_costable[i]*M_PI/(2.0*N)));
*/
    for (int i=0; i<X; i++) {
      C2[0][i] = 1.0 / sqrt((double)X);
      Ct2[i][0] = C2[0][i];
    }

    for (int i=1; i<X; i++) {
      for (int j=0; j<Y; j++) {
        C2[i][j] = sqrt(2.0 / X) * cos( M_PI * (2 * j + 1) * i / (2.0 * X));
        Ct2[j][i] = C2[i][j];
      }
    }


}
