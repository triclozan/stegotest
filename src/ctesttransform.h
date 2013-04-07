#ifndef CTESTTRANSFORM_H
#define CTESTTRANSFORM_H
#include "ctest.h"
#include <QByteArray>
#include <list>
#include "util.h"
using namespace std;

class CTestTransform : public CTest
{
public:
    struct sTransform {
        QByteArray params;
        int id;
    };
    struct fParams {
        int N;
        int rep;
    };
    struct shParams {
        double strength;
        int rep;
    };

    //CTestTransform(sTransform* transforms, int count);
    CTestTransform();
    void addTransform(sTransform t);
    virtual void Attack(QImage& img, QString& params);
private:
    list<sTransform> transforms;
    void performTransform(QImage& img, QImage& out, sTransform& t);
};

#endif // CTESTTRANSFORM_H
