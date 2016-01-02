#include "transfer_control_point.h"

namespace core
{

TransferControlPoint::TransferControlPoint()
: mColor(QVector4D())
, mDataSetValue(0)
{

}

TransferControlPoint::TransferControlPoint(float red, float green, float blue, float dataSetValue)
{
    mColor.setX(red);
    mColor.setY(green);
    mColor.setZ(blue);
    mColor.setW(1);

    mDataSetValue = dataSetValue;
}

TransferControlPoint::TransferControlPoint(float alpha, float dataSetValue)
{
    mColor.setX(0);
    mColor.setY(0);
    mColor.setZ(0);
    mColor.setW(alpha);

    mDataSetValue = dataSetValue;

}

TransferControlPoint::~TransferControlPoint()
{

}

QVector4D TransferControlPoint::getColor() const
{
    return mColor;
}

float TransferControlPoint::getDataSetValue() const
{
    return mDataSetValue;
}

} // namespace core
