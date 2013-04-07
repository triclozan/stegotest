#ifndef CIMGCOMPARE_H
#define CIMGCOMPARE_H

class QImage;
class CImgCompare
{
public:
    struct Result {
        double md;
        double ad;
        double nad;
        double mse;
        double nmse;
        double l2;
        double snr;
        double psnr;
        double ifid;
        double nc;
        double cq;
        double sc;
        double hs;
    };


    CImgCompare();
    static Result PSNR_B(QImage& orig, QImage& res, int channel = 0);

};

#endif // CIMGCOMPARE_H
