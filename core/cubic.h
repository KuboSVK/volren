#ifndef CUBIC_H
#define CUBIC_H

#include <QVector4D>

namespace core
{

class Cubic
{
public:
    Cubic();
    Cubic(QVector4D a, QVector4D b, QVector4D c, QVector4D d);

    /*
        Ziskam bod leziaci na splajne v konkretnom segmente vyhodnotenim
        a + b*u + c*u^2 + d*u^3 pre lubovolne u z intervalu <0, 1)
    */
    QVector4D getPointOnSpline(float u);

private:
    QVector4D mA, mB, mC, mD; // a + b*u + c*u^2 + d*u^3
};

} // namespace core

#endif // CUBIC_H
