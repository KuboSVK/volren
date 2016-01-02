#include "ray_casting_engine.h"

#include <QMessageBox>
#include <QVector4D>
#include <stdio.h>

#include <QDebug>

#include "cubic.h"
#include "cubic_spline.h"

namespace core
{

RayCastingEngine::RayCastingEngine()
: mWidth(800)
, mHeight(600)
, mCurrentVolumeData(NULL)
, mMinVolumeDataValue(0)
, mMaxVolumeDataValue(0)
, m1DTransferFucntion(NULL)
{
    VolumeDataDesciptor aneurism
    {
        QVector3D(256, 256, 256), // grid size per dimension
        quint32(256 * 256 * 256), // cummulative grid size
        QVector3D(1, 1, 1) // grid sample ration - data z www.volvis.org su reprezentovane na ekvidistantnej mriezke
    };

    VolumeDataDesciptor bonsai
    {
        QVector3D(256, 256, 256), // grid size per dimension
        quint32(256 * 256 * 256), // cummulative grid size
        QVector3D(1, 1, 1) // grid sample ration - data z www.volvis.org su reprezentovane na ekvidistantnej mriezke
    };

    VolumeDataDesciptor bostonTeapot
    {
        QVector3D(256, 256, 178), // grid size per dimension
        quint32(256 * 256 * 178), // cummulative grid size
        QVector3D(1, 1, 1) // grid sample ration - data z www.volvis.org su reprezentovane na ekvidistantnej mriezke
    };

    VolumeDataDesciptor male
    {
        QVector3D(128, 256, 256), // grid size per dimension
        quint32(128 * 256 * 256), // cummulative grid size
        QVector3D(1.57774, 0.995861, 1.00797) // ne-ekvidistantna mriezka
    };

    mVolumeDataDescription.insert("aneurism", aneurism);
    mVolumeDataDescription.insert("bonsai", bonsai);
    mVolumeDataDescription.insert("BostonTeapot", bostonTeapot);
    mVolumeDataDescription.insert("male", male);
}

RayCastingEngine::~RayCastingEngine()
{
    delete[] mCurrentVolumeData;
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

void RayCastingEngine::loadRawVolumeData(QString volumeDataFilePath)
{
    if (!volumeDataFilePath.isEmpty())
    {
        FILE* fp = fopen(volumeDataFilePath.toLatin1().data(), "rb");
        if (fp == NULL)
        {
            QMessageBox::critical(0, "Error", "Could not open volume data file!");
        }
        else
        {
            QString volumeDataFileName = volumeDataFilePath.split("/", QString::SkipEmptyParts, Qt::CaseInsensitive).last();
            if (volumeDataFileName == "aneurism.raw")
            {
                mCurrentVolumeDataDescriptor = mVolumeDataDescription["aneurism"];
            }
            else if (volumeDataFileName == "bonsai.raw")
            {
                mCurrentVolumeDataDescriptor = mVolumeDataDescription["bonsai"];
            }
            else if (volumeDataFileName == "BostonTeapot.raw")
            {
                mCurrentVolumeDataDescriptor = mVolumeDataDescription["BostonTeapot"];
            }
            else if (volumeDataFileName == "male.raw")
            {
                mCurrentVolumeDataDescriptor = mVolumeDataDescription["male"];
            }

            if (mCurrentVolumeData != NULL)
            {
                delete[] mCurrentVolumeData;
                mCurrentVolumeData = NULL;
            }

            mCurrentVolumeData = new GLubyte[mCurrentVolumeDataDescriptor.mCummulativeGridSize];
            fread((void*)mCurrentVolumeData, sizeof(GLubyte), mCurrentVolumeDataDescriptor.mCummulativeGridSize, fp);
            fclose(fp);

            prepareVolumeData();
        }
    }
}

void RayCastingEngine::prepareVolumeData()
{
    mMinVolumeDataValue = mCurrentVolumeData[0];
    mMaxVolumeDataValue = mCurrentVolumeData[0];

    for (quint32 i = 1; i < mCurrentVolumeDataDescriptor.mCummulativeGridSize; ++i)
    {
        if (mCurrentVolumeData[i] < mMinVolumeDataValue)
        {
            mMinVolumeDataValue = mCurrentVolumeData[i];
        }

        if (mCurrentVolumeData[i] > mMaxVolumeDataValue)
        {
            mMaxVolumeDataValue = mCurrentVolumeData[i];
        }
    }

    createSplinePoints();
    computeTransferFunction();
}

void RayCastingEngine::createSplinePoints()
{
    /*
        Vytvorenie bodov pre cubic spline interpolaciu - definicia jednotlivych materialov v data set a im prisluchajucich hodnot RGBA.
        Pre colorPoints a alphaPoints treba mat RGBA hodnoty pre isovalue 0 i 255 - inak by prebehla interpolacia chybne.
        Nastavenie tychto hodnot, resp. tvorba transfer function je rucna a dlhodoba praca, treba skusat a najst tie spravne hodnoty.
        Ina moznost je to realizovat realtime - nateraz pracne --> TO DO neskor
    */

    // nastavenie pre male.raw
    // skin sa pohybuje v intervale priblizne <40, 70>
    // bone sa pohybuje v intervale priblizne <70, 255>
    mColorPoints.clear();
    mAlphaPoints.clear();

    mColorPoints.push_back(TransferControlPoint(0.91f, 0.7f, 0.61f, 0.0f)); // color for skin
    mColorPoints.push_back(TransferControlPoint(0.91f, 0.7f, 0.61f, 80.0f)); // color for skin
    mColorPoints.push_back(TransferControlPoint(1.0f, 1.0f, 0.85f, 82.0f)); // color for bone
    mColorPoints.push_back(TransferControlPoint(1.0f, 1.0f, 0.85f, 255.0f)); // color for bone

    mAlphaPoints.push_back(TransferControlPoint(0.0f, 0.0f));
    mAlphaPoints.push_back(TransferControlPoint(0.0f, 40.0f));
    mAlphaPoints.push_back(TransferControlPoint(0.2f, 60.0f));
    mAlphaPoints.push_back(TransferControlPoint(0.05f, 63.0f));
    mAlphaPoints.push_back(TransferControlPoint(0.0f, 80.0f));
    mAlphaPoints.push_back(TransferControlPoint(0.9f, 82.0f));
    mAlphaPoints.push_back(TransferControlPoint(1.0f, 255.0f));
}

void RayCastingEngine::computeTransferFunction()
{
    // interpolovane hodnoty RGBA
    QVector4D interpolatedValues[256];

    Cubic* colorCubic = CubicSpline::computeCubicSpline(mColorPoints.size() - 1, mColorPoints);
    Cubic* alphaCubic = CubicSpline::computeCubicSpline(mAlphaPoints.size() - 1, mAlphaPoints);

    qint32 index = 0;
    for (qint32 i = 0; i < mColorPoints.size() - 1; i++)
    {
        qint32 nSteps = mColorPoints[i + 1].getDataSetValue() - mColorPoints[i].getDataSetValue();

        for (qint32 j = 0; j < nSteps; j++)
        {
            float parameter = (float)j / (float)(nSteps - 1);
            interpolatedValues[index++] = colorCubic[i].getPointOnSpline(parameter);
        }
    }

    index = 0;
    for (qint32 i = 0; i < mAlphaPoints.size() - 1; i++)
    {
        qint32 nSteps = mAlphaPoints[i + 1].getDataSetValue() - mAlphaPoints[i].getDataSetValue();

        for (qint32 j = 0; j < nSteps; j++)
        {
            float parameter = (float)j / (float)(nSteps - 1);
            interpolatedValues[index++].setW(alphaCubic[i].getPointOnSpline(parameter).w());
        }
    }

    delete[] colorCubic;
    delete[] alphaCubic;

    if (m1DTransferFucntion != NULL)
    {
        delete[] m1DTransferFucntion;
        m1DTransferFucntion = NULL;
    }

    m1DTransferFucntion = new GLfloat[256 * 4];
    for (int i = 0; i < 256; i++)
    {
        m1DTransferFucntion[(i * 4) + 0] = interpolatedValues[i].x();
        qDebug() << QString("Value[%1] = %2").arg(i).arg(m1DTransferFucntion[(i * 4) + 0]);

        m1DTransferFucntion[(i * 4) + 1] = interpolatedValues[i].y();
        qDebug() << QString("Value[%1] = %2").arg(i).arg(m1DTransferFucntion[(i * 4) + 1]);

        m1DTransferFucntion[(i * 4) + 2] = interpolatedValues[i].z();
        qDebug() << QString("Value[%1] = %2").arg(i).arg(m1DTransferFucntion[(i * 4) + 2]);

        m1DTransferFucntion[(i * 4) + 3] = interpolatedValues[i].w();
        qDebug() << QString("Value[%1] = %2").arg(i).arg(m1DTransferFucntion[(i * 4) + 3]);
    }
}

} // namespace core
