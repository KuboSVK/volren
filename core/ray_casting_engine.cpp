#include "ray_casting_engine.h"

#include <QDataStream>
#include <QFile>
#include <QMessageBox>

namespace core
{

RayCastingEngine::RayCastingEngine()
: mWidth(800)
, mHeight(600)
, mVolumeData(0)
{
    VolumeDataDesciptor aneurism
    {
        QVector3D(256, 256, 256), // grid size
        QVector3D(1, 1, 1) // grid sample ration - data z www.volvis.org su reprezentovane na ekvidistantnej mriezke
    };

    VolumeDataDesciptor bonsai
    {
        QVector3D(256, 256, 256), // grid size
        QVector3D(1, 1, 1) // grid sample ration - data z www.volvis.org su reprezentovane na ekvidistantnej mriezke
    };

    VolumeDataDesciptor bostonTeapot
    {
        QVector3D(256, 256, 178), // grid size
        QVector3D(1, 1, 1) // grid sample ration - data z www.volvis.org su reprezentovane na ekvidistantnej mriezke
    };

    VolumeDataDesciptor male
    {
        QVector3D(128, 256, 256), // grid size
        QVector3D(1.57774, 0.995861, 1.00797) // ne-ekvidistantna mriezka
    };

    mVolumeDataDescription.insert("aneurism", aneurism);
    mVolumeDataDescription.insert("bonsai", bonsai);
    mVolumeDataDescription.insert("BostonTeapot", bostonTeapot);
    mVolumeDataDescription.insert("male", male);
}

RayCastingEngine::~RayCastingEngine()
{

}

void RayCastingEngine::resize(int width, int height)
{

}

void RayCastingEngine::init()
{
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
    //glDisable(GL_DEPTH_TEST);
    glDisable(GL_COLOR_MATERIAL);
}

void RayCastingEngine::draw()
{

}

void RayCastingEngine::loadRawVolumeData(QString volumeDataFile)
{
    if (!volumeDataFile.isEmpty())
    {
        QFile dataFile(volumeDataFile);

        if (!dataFile.open(QIODevice::ReadOnly))
        {
            QMessageBox::critical(0, "Error", "Could not open volume data file!");
        }
        else
        {
            QDataStream dataStream(&dataFile);
            prepareVolumeData(dataStream);
        }
        dataFile.close();
    }
}

void RayCastingEngine::prepareVolumeData(const QDataStream &volumeDataStream)
{
    //mVolumeData = new GLubyte[gridSize.x() * gridSize.y() * gridSize.z()];

}

} // namespace core
