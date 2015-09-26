#ifndef CTESTTRANSFORM_H
#define CTESTTRANSFORM_H
#include "ctest.h"
using namespace std;

class CTestTransform : public CTest
{
public:
    CTestTransform();
    virtual void Attack(QImage& img, QString& params);
};

#endif // CTESTTRANSFORM_H
