#include "cubic_spline.h"

#include <QVector4D>

#include "cubic.h"

namespace core
{

Cubic* CubicSpline::computeCubicSpline(quint32 nPoints, QVector<TransferControlPoint> points)
{
    QVector4D* gamma = new QVector4D[nPoints + 1];
    QVector4D* delta = new QVector4D[nPoints + 1];
    QVector4D* D = new QVector4D[nPoints + 1];

    /*
    Riesime sustavu prevzatu z http://mathworld.wolfram.com/CubicSpline.html .
           [2 1       ] [D[0]]          [3(points[1] - points[0])  ]
           |1 4 1     | |D[1]|          |3(points[2] - points[0])  |
           |  1 4 1   | | .  | =		|      .				   |
           |    ..... | | .  |			|      .				   |
           |     1 4 1| | .  |			|3(points[nPoints] - points[nPoints-2])|
           [       1 2] [D[nPoints]]	[3(points[nPoints] - points[nPoints-1])]
   */

    QVector4D v(1, 1, 1, 1);
    gamma[0].setX(1.0f / 2.0f);
    gamma[0].setY(1.0f / 2.0f);
    gamma[0].setZ(1.0f / 2.0f);
    gamma[0].setW(1.0f / 2.0f);

    for (quint32 i = 1; i < nPoints; i++)
    {
        gamma[i] = v / ((v * 4.0f) - gamma[i - 1]);
    }
    gamma[nPoints] = v / ((v * 2.0f) - gamma[nPoints - 1]);


    delta[0] = (points[1].getColor() - points[0].getColor()) * 3.0f * gamma[0];
    for (quint32 i = 1; i < nPoints; i++)
    {
        delta[i] = ((points[i + 1].getColor() - points[i - 1].getColor()) * 3.0f - delta[i - 1]) * gamma[i];
    }
    delta[nPoints] = ((points[nPoints].getColor() - points[nPoints - 1].getColor()) * 3.0f - delta[nPoints - 1]) * gamma[nPoints];

    D[nPoints] = delta[nPoints];
    for (qint32 i = nPoints - 1; i >= 0; i--)
    {
        D[i] = delta[i] - gamma[i] * D[i + 1];
    }

    // ako posledny krok ideme spocitat koeficienty jednotlivych kubik pre jednotlive segmenty
    Cubic* C = new Cubic[nPoints];
    for (quint32 i = 0; i < nPoints; i++)
    {
        C[i] = Cubic(points[i].getColor(), D[i], (points[i + 1].getColor() - points[i].getColor()) * 3.0f - D[i] * 2.0f - D[i + 1],
            (points[i].getColor() - points[i + 1].getColor()) * 2.0f + D[i] + D[i + 1]);
    }

    delete[] gamma;
    delete[] delta;
    delete[] D;

    return C;
}

} // namespace core
