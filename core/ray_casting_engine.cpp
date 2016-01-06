#include "ray_casting_engine.h"

#include <QFile>
#include <QMessageBox>
#include <QVector4D>
#include <stdio.h>

#include "cubic.h"
#include "cubic_spline.h"
#include "shader_object.h"
#include "program_object.h"

#include <QDebug>

namespace core
{

RayCastingEngine::RayCastingEngine()
: mCurrentVolumeData(NULL)
, m1DTransferFucntion(NULL)
{
    VolumeDataDesciptor aneurism
    {
        QVector3D(256, 256, 256), // grid size per dimension
        quint32(256 * 256 * 256), // cummulative grid size
        QVector3D(1, 1, 1), // grid sample ration - data z www.volvis.org su reprezentovane na ekvidistantnej mriezke
        QVector3D(), // mBoundingBoxScaleFactor - inicializacia prebehne neskor
        QVector3D(), // mBoundingBoxCenter - inicializacia prebehne neskor
        float(), // mStepSize - inicializacia prebehne neskor
        quint32() // mIterations - inicializacia prebehne neskor
    };

    VolumeDataDesciptor bonsai
    {
        QVector3D(256, 256, 256), // grid size per dimension
        quint32(256 * 256 * 256), // cummulative grid size
        QVector3D(1, 1, 1), // grid sample ration - data z www.volvis.org su reprezentovane na ekvidistantnej mriezke
        QVector3D(), // mBoundingBoxScaleFactor - inicializacia prebehne neskor
        QVector3D(), // mBoundingBoxCenter - inicializacia prebehne neskor
        float(), // mStepSize - inicializacia prebehne neskor
        quint32() // mIterations - inicializacia prebehne neskor
    };

    VolumeDataDesciptor bostonTeapot
    {
        QVector3D(256, 256, 178), // grid size per dimension
        quint32(256 * 256 * 178), // cummulative grid size
        QVector3D(1, 1, 1), // grid sample ration - data z www.volvis.org su reprezentovane na ekvidistantnej mriezke
        QVector3D(), // mBoundingBoxScaleFactor - inicializacia prebehne neskor
        QVector3D(), // mBoundingBoxCenter - inicializacia prebehne neskor
        float(), // mStepSize - inicializacia prebehne neskor
        quint32() // mIterations - inicializacia prebehne neskor
    };

    VolumeDataDesciptor male
    {
        QVector3D(128, 256, 256), // grid size per dimension
        quint32(128 * 256 * 256), // cummulative grid size
        QVector3D(1.57774, 0.995861, 1.00797), // ne-ekvidistantna mriezka
        QVector3D(), // mBoundingBoxScaleFactor - inicializacia prebehne neskor
        QVector3D(), // mBoundingBoxCenter - inicializacia prebehne neskor
        float(), // mStepSize - inicializacia prebehne neskor
        quint32() // mIterations - inicializacia prebehne neskor
    };

    mVolumeDataSets.insert("aneurism", aneurism);
    mVolumeDataSets.insert("bonsai", bonsai);
    mVolumeDataSets.insert("BostonTeapot", bostonTeapot);
    mVolumeDataSets.insert("male", male);

    preprocessVolumeData();
}

RayCastingEngine::~RayCastingEngine()
{
    delete[] mCurrentVolumeData;
    delete[] m1DTransferFucntion;
}

void RayCastingEngine::resize(int width, int height)
{

}

void RayCastingEngine::init()
{
    initializeShaders();
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
                mCurrentVolumeDataSet = "aneurism";
            }
            else if (volumeDataFileName == "bonsai.raw")
            {
                mCurrentVolumeDataSet = "bonsai";
            }
            else if (volumeDataFileName == "BostonTeapot.raw")
            {
                mCurrentVolumeDataSet = "BostonTeapot";
            }
            else if (volumeDataFileName == "male.raw")
            {
                mCurrentVolumeDataSet = "male";
            }

            if (mCurrentVolumeData != NULL)
            {
                delete[] mCurrentVolumeData;
                mCurrentVolumeData = NULL;
            }

            mCurrentVolumeData = new GLubyte[mVolumeDataSets[mCurrentVolumeDataSet].mCummulativeGridSize];
            fread((void*)mCurrentVolumeData, sizeof(GLubyte), mVolumeDataSets[mCurrentVolumeDataSet].mCummulativeGridSize, fp);
            fclose(fp);

            GLubyte mMinVolumeDataValue = mCurrentVolumeData[0];
            GLubyte mMaxVolumeDataValue = mCurrentVolumeData[0];

            for (quint32 i = 1; i < mVolumeDataSets[mCurrentVolumeDataSet].mCummulativeGridSize; ++i)
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
    }
}

void RayCastingEngine::preprocessVolumeData()
{
    // vypocet scale faktorov
    // nas bounding box je jednotkova kocka zarovnana vzhladom k hlavnym suradnicovym osiam x, y, z v object - space
    // jeden extremny vrchol bounding boxu sa nachadza v pociatku suradnicovej sustavy, druhy v bode o suradniciach (1, 1, 1)
    // 3D data volumes nie su vzdy idealne kocky - je potrebne scalnut nas bounding box (proxy geometry) hodnotami vo vektore scaleFactor
    // takisto sa moze stat, ze skalarne data nie su rozprestrene na mriezke s ekvidistantnymi vzdialenostami - musime prenasobit velkosti volume modelu vektorom gridSampleRatio

    // vypocet scale faktoru, ide vlastne o normalized volume extend - tento vektor sa pouzije ako uniform premenna vo vertex shaderi, kde nim prenasobim poziciu kazdeho vrcholu nasej bounding box
    QMap<QString, VolumeDataDesciptor>::iterator it = mVolumeDataSets.begin();
    while (it != mVolumeDataSets.end())
    {
        VolumeDataDesciptor volumeDataSet = it.value();
        QVector3D gridSizePerDimension = volumeDataSet.mGridSizePerDimension;
        QVector3D gridSampleRatio = volumeDataSet.mGridSampleRatio;
        float maxSizePerDimension = qMax(gridSizePerDimension.x(), qMax(gridSizePerDimension.y(), gridSizePerDimension.z()));

        volumeDataSet.mBoundingBoxScaleFactor.setX((float) (1.0 / (maxSizePerDimension / (gridSizePerDimension.x() * gridSampleRatio.x()))));
        volumeDataSet.mBoundingBoxScaleFactor.setY((float) (1.0 / (maxSizePerDimension / (gridSizePerDimension.y() * gridSampleRatio.y()))));
        volumeDataSet.mBoundingBoxScaleFactor.setZ((float) (1.0 / (maxSizePerDimension / (gridSizePerDimension.z() * gridSampleRatio.z()))));

        volumeDataSet.mBoundingBoxCenter.setX(volumeDataSet.mBoundingBoxScaleFactor.x() / 2);
        volumeDataSet.mBoundingBoxCenter.setY(volumeDataSet.mBoundingBoxScaleFactor.y() / 2);
        volumeDataSet.mBoundingBoxCenter.setZ(volumeDataSet.mBoundingBoxScaleFactor.z() / 2);

        volumeDataSet.mStepSize = 1.0 / maxSizePerDimension;
        volumeDataSet.mIterations = maxSizePerDimension * 2;

        ++it;
    }
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

void RayCastingEngine::initializeShaders()
{
    QFile vertexShaderFile(":/shaders/rayCasting.vert");
    QFile fragmentShaderFile(":/shaders/rayCasting.frag");

    if (!vertexShaderFile.open(QIODevice::ReadOnly | QIODevice::Text) || !fragmentShaderFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(0, "Error", "Could not open vertex/fragment shader file!");
    }
    else
    {
        QString vertexShaderSource = QString(vertexShaderFile.readAll().data());
        QString fragmentShaderSource = QString(fragmentShaderFile.readAll().data());

        vertexShaderFile.close();
        fragmentShaderFile.close();

        ShaderObject vertexShaderObject(GL_VERTEX_SHADER, vertexShaderSource);
        ShaderObject fragmentShaderObject(GL_FRAGMENT_SHADER, fragmentShaderSource);

        if (!vertexShaderObject.setShaderObjectSource() || !fragmentShaderObject.setShaderObjectSource())
        {
            QMessageBox::critical(0, "Error", "Could not set source code for vertex/fragment shader object!");
        }
        else
        {
            if (!vertexShaderObject.compileShader() || !fragmentShaderObject.compileShader())
            {
                QMessageBox::critical(0, "Error", "Could not compile vertex/fragment shader object!");
            }
            else
            {
                QList<ShaderObject*> shaderObjectList
                {
                    &vertexShaderObject,
                    &fragmentShaderObject
                };

                ProgramObject programObject(shaderObjectList);

                if (!programObject.attachShaderObjects())
                {
                    QMessageBox::critical(0, "Error", "Could not attach compiled shader objects to the program object!");
                }
                else
                {
                    if (!programObject.linkProgramObject())
                    {
                        QMessageBox::critical(0, "Error", "Could not link program object!");
                    }
                }
            }
        }
    }
}

} // namespace core
