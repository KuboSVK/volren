#ifndef RAY_CASTING_ENGINE_H
#define RAY_CASTING_ENGINE_H

#include <QMap>
#include <QOpenGLFunctions>
#include <QString>
#include <QVector>
#include <QVector3D>

#include "irenderer.h"
#include "transfer_control_point.h"

namespace core
{

struct VolumeDataDesciptor
{
    QVector3D mGridSizePerDimension;
    quint32 mCummulativeGridSize;
    QVector3D mGridSampleRatio;
    QVector3D mBoundingBoxScaleFactor;
    QVector3D mBoundingBoxCenter;
    float mStepSize;
    quint32 mIterations;
};

class RayCastingEngine : public IRenderer, protected QOpenGLFunctions
{
public:
    RayCastingEngine();
    ~RayCastingEngine();

    virtual void resize(int width, int height) Q_DECL_OVERRIDE;
    virtual void init() Q_DECL_OVERRIDE;
    virtual void draw() Q_DECL_OVERRIDE;
    virtual void loadRawVolumeData(QString volumeDataFilePath) Q_DECL_OVERRIDE;

private:
    void preprocessVolumeData();
    void createSplinePoints();
    void computeTransferFunction();
    void initializeShaders();

private:
    GLubyte* mCurrentVolumeData;
    GLfloat* m1DTransferFucntion;

    QString mCurrentVolumeDataSet;
    QMap<QString, VolumeDataDesciptor> mVolumeDataSets;

    QVector<TransferControlPoint> mColorPoints;
    QVector<TransferControlPoint> mAlphaPoints;
};

} //namespace core

#endif // RAY_CASTING_ENGINE_H
