#ifndef PROGRAM_OBJECT_H
#define PROGRAM_OBJECT_H

#include <QList>
#include <QOpenGLFunctions>

namespace core
{

// forward declaration(s)
class ShaderObject;

class ProgramObject : public QOpenGLFunctions
{
public:
    ProgramObject(const QList<ShaderObject*> shaderObjects);
    ~ProgramObject();

    bool attachShaderObjects();
    bool linkProgramObject();

private:
    GLuint mProgramObjectId;
    const QList<ShaderObject*> mAttachedShaderObjects;

    Q_DISABLE_COPY(ProgramObject)
};

} // namespace core

#endif // PROGRAM_OBJECT_H
