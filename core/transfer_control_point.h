#ifndef TRANSFER_CONTROL_POINT_H
#define TRANSFER_CONTROL_POINT_H

#include <QVector4D>

namespace core
{

/*
    Trieda reprezentujuca jednotlive body 1D transfer function pouzitej v procese klasifikacie.
    Kazdy bod pre konkretnu skalarnu hodnotu z data set uchovava tomu zodpovedajucu hodnotu rgba.
    Tymito bodmi neskor prelozime natural cubic spline cim docielime interpolaciu medzi tymito bodmi, resp. ich rgba hodnotami.
    Z tychto interpolovanych hodnot zostrojime 1D texturu, ktoru pouzijeme pri mapovani data set value -> rgba vo fragment shaderi.
*/
class TransferControlPoint
{
public:
    TransferControlPoint();
    TransferControlPoint(float red, float green, float blue, float dataSetValue);
    TransferControlPoint(float alpha, float dataSetValue);
    ~TransferControlPoint();

    QVector4D getColor() const;
    float getDataSetValue() const;

private:
    QVector4D mColor;
    float mDataSetValue;
};

} //namespace core

#endif // TRANSFER_CONTROL_POINT_H
