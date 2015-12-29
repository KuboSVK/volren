#include "shader_object.h"

#include <QDebug>

namespace core
{

ShaderObject::ShaderObject(GLenum shaderType, const QString shaderSourceString)
: mShaderType(shaderType)
, mShaderSourceString(shaderSourceString)
{

}

ShaderObject::~ShaderObject()
{

}

bool ShaderObject::compileShaderObject()
{
    bool result(false);

    mShaderObjectId = glCreateShader(mShaderType);
    if (mShaderObjectId != 0)
    {
        const char* shaderSource = mShaderSourceString.toLatin1().data();
        glShaderSource(mShaderObjectId, 1, &shaderSource, NULL);

        GLint shaderSourceLength;
        glGetShaderiv(mShaderObjectId, GL_SHADER_SOURCE_LENGTH, &shaderSourceLength);
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

    glCompileShader(mShaderObjectId);

    GLint compileStatus;
    glGetShaderiv(mShaderObjectId, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE)
    {
        GLint infoLogLength;
        glGetShaderiv(mShaderObjectId, GL_INFO_LOG_LENGTH, &infoLogLength);

        char* infoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(mShaderObjectId, infoLogLength, NULL, infoLog);

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
