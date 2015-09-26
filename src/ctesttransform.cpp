#include "ctesttransform.h"
#include "ctransform.h"
using namespace std;

CTestTransform::CTestTransform() {}

void CTestTransform::Attack(QImage& in, QString& params)
{
    CTransform t;
    t.performTransform(in, params);
}
