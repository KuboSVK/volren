#ifndef RAY_CASTING_ENGINE_H
#define RAY_CASTING_ENGINE_H

#include <QObject>
#include <QMap>
#include <QOpenGLFunctions>
#include <QString>
#include <QVector3D>

#include "irenderer.h"

QT_FORWARD_DECLARE_CLASS(QDataStream)

namespace core
{

struct VolumeDataDesciptor
{
    QVector3D mGridSize;
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
    virtual void loadRawVolumeData(QString volumeDataFile) Q_DECL_OVERRIDE;

private:
    void prepareVolumeData(const QDataStream& volumeDataStream);

private:
    int mWidth;
    int mHeight;

    GLubyte* mVolumeData;
    QMap<QString, VolumeDataDesciptor> mVolumeDataDescription;
};

} //namespace core

#endif // RAY_CASTING_ENGINE_H
