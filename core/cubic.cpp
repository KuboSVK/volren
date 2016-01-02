#include "cubic.h"

namespace core
{

Cubic::Cubic()
: mA(QVector4D())
, mB(QVector4D())
, mC(QVector4D())
, mD(QVector4D())
{

}

Cubic::Cubic(QVector4D a, QVector4D b, QVector4D c, QVector4D d)
: mA(a)
, mB(b)
, mC(c)
, mD(d)
{

}

QVector4D Cubic::getPointOnSpline(float u)
{
    //return (((mD * u) + mC) *u + mB) *u + mA;
    return QVector4D();
}

} // namespace
