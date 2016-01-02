#ifndef CUBIC_SPLINE_H
#define CUBIC_SPLINE_H

#include <QVector>

#include "transfer_control_point.h"

namespace core
{

// forward declaration(s)
class Cubic;

class CubicSpline
{
public:
    // vypocet natural cubic spline prelozenim zadanymi bodmi (TransferControlPoint)
    static Cubic* computeCubicSpline(quint32 nPoints, QVector<TransferControlPoint> points);
};

} // namespace core

#endif // CUBIC_SPLINE_H
