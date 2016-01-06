#ifndef SHADER_OBJECT_H
#define SHADER_OBJECT_H

#include <QOpenGLFunctions>
#include <QString>

namespace core
{

class ShaderObject : public QOpenGLFunctions
{
public:
    ShaderObject(GLenum shaderType, const QString shaderSourceString);
    ~ShaderObject();

    bool setShaderObjectSource();
    bool compileShader();
    GLuint shaderObjectId() const;

private:
    GLenum mShaderType;
    QString mShaderSourceString;
    GLuint mShaderObjectId;

    QOpenGLFunctions* mOpenGLFunctions;

    Q_DISABLE_COPY(ShaderObject)
};

} // namespace core

#endif // SHADER_OBJECT_H
