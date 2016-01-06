#include "shader_object.h"

#include <QDebug>

namespace core
{

ShaderObject::ShaderObject(GLenum shaderType, const QString shaderSourceString)
: mShaderType(shaderType)
, mShaderSourceString(shaderSourceString)
, mOpenGLFunctions(QOpenGLContext::currentContext()->functions())
{

}

ShaderObject::~ShaderObject()
{

}

bool ShaderObject::setShaderObjectSource()
{
    bool result(false);

    mShaderObjectId = mOpenGLFunctions->glCreateShader(mShaderType);
    if (mShaderObjectId != 0)
    {
        const char* shaderSource = mShaderSourceString.toLatin1().data();
        mOpenGLFunctions->glShaderSource(mShaderObjectId, 1, &shaderSource, NULL);

        GLint shaderSourceLength;
        mOpenGLFunctions->glGetShaderiv(mShaderObjectId, GL_SHADER_SOURCE_LENGTH, &shaderSourceLength);
        if (shaderSourceLength != 0)
        {
            result = true;
        }
    }

    return result;
}

bool ShaderObject::compileShader()
{
    bool result(true);

    mOpenGLFunctions->glCompileShader(mShaderObjectId);

    GLint compileStatus;
    mOpenGLFunctions->glGetShaderiv(mShaderObjectId, GL_COMPILE_STATUS, &compileStatus);
    qDebug() << compileStatus;
    if (compileStatus == GL_FALSE)
    {
        GLint infoLogLength;
        mOpenGLFunctions->glGetShaderiv(mShaderObjectId, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* infoLog = new char[infoLogLength + 1];
        mOpenGLFunctions->glGetShaderInfoLog(mShaderObjectId, infoLogLength, NULL, infoLog);

        qDebug() << "Shader compilation failed.\n";
        qDebug() << infoLog;

        delete[] infoLog;

        result = false;
    }

    return result;
}

GLuint ShaderObject::shaderObjectId() const
{
    return mShaderObjectId;
}

} // namespace core
