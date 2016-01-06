#include "program_object.h"

#include <QDebug>

#include "shader_object.h"

namespace core
{

ProgramObject::ProgramObject(const QList<ShaderObject*> shaderObjects)
: mAttachedShaderObjects(shaderObjects)
, mOpenGLFunctions(QOpenGLContext::currentContext()->functions())
{

}

ProgramObject::~ProgramObject()
{

}

bool ProgramObject::attachShaderObjects()
{
    bool result(false);

    mProgramObjectId = mOpenGLFunctions->glCreateProgram();
    if (mProgramObjectId != 0)
    {
        foreach (const ShaderObject* shaderObject, mAttachedShaderObjects)
        {
            mOpenGLFunctions->glAttachShader(mProgramObjectId, shaderObject->shaderObjectId());
        }

        result = true;
    }

    return result;
}

bool ProgramObject::linkProgramObject()
{
    bool result;

    mOpenGLFunctions->glLinkProgram(mProgramObjectId);

    GLint linkStatus;
    mOpenGLFunctions->glGetProgramiv(mProgramObjectId, GL_LINK_STATUS, &linkStatus);
    if (linkStatus == GL_TRUE)
    {
        foreach (const ShaderObject* shaderObject, mAttachedShaderObjects) {
            mOpenGLFunctions->glDetachShader(mProgramObjectId, shaderObject->shaderObjectId());
        }

        result = true;
    }
    else // linkStatus == GL_FALSE
    {
        GLint infoLogLength;
        mOpenGLFunctions->glGetProgramiv(mProgramObjectId, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* infoLog = new char[infoLogLength + 1];
        mOpenGLFunctions->glGetProgramInfoLog(mProgramObjectId, infoLogLength, NULL, infoLog);

        qDebug() << "Linking failure.\n";
        qDebug() << infoLog;

        delete[] infoLog;

        result = false;
    }

    return result;
}

} // namespace core
