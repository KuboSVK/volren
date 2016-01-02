#ifndef RAY_CASTING_ENGINE_H
#define RAY_CASTING_ENGINE_H

#include <QByteArray>
#include <QObject>
#include <QMap>
#include <QOpenGLFunctions>
#include <QString>
#include <QVector>
#include <QVector3D>

#include "irenderer.h"
#include "transfer_control_point.h"

QT_FORWARD_DECLARE_CLASS(QDataStream)

namespace core
{

struct VolumeDataDesciptor
{
    QVector3D mGridSizePerDimension;
    quint32 mCummulativeGridSize;
    QVector3D mGridSampleRatio;
};

class RayCastingEngine : public IRenderer, public QOpenGLFunctions
{
public:
    RayCastingEngine();
    ~RayCastingEngine();

    virtual void resize(int width, int height) Q_DECL_OVERRIDE;
    virtual void init() Q_DECL_OVERRIDE;
    virtual void draw() Q_DECL_OVERRIDE;
    virtual void loadRawVolumeData(QString volumeDataFilePath) Q_DECL_OVERRIDE;

private:
    void prepareVolumeData();
    void createSplinePoints();
    void computeTransferFunction();

private:
    int mWidth;
    int mHeight;

    GLubyte* mCurrentVolumeData;
    GLubyte mMinVolumeDataValue;
    GLubyte mMaxVolumeDataValue;

    GLfloat* m1DTransferFucntion;

    VolumeDataDesciptor mCurrentVolumeDataDescriptor;
    QMap<QString, VolumeDataDesciptor> mVolumeDataDescription;

    QVector<TransferControlPoint> mColorPoints;
    QVector<TransferControlPoint> mAlphaPoints;
};

} //namespace core

#endif // RAY_CASTING_ENGINE_H
