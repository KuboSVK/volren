#ifndef RAY_CASTING_ENGINE_H
#define RAY_CASTING_ENGINE_H

#include <QObject>
#include <QOpenGLFunctions>

#include "irenderer.h"

namespace core
{

class RayCastingEngine : public IRenderer, public QOpenGLFunctions
{
public:
    RayCastingEngine();
    ~RayCastingEngine();

    virtual void resize(int width, int height) Q_DECL_OVERRIDE;
    virtual void init() Q_DECL_OVERRIDE;
    virtual void draw() Q_DECL_OVERRIDE;

private:
    int mWidth;
    int mHeight;
};

} //namespace core

#endif // RAY_CASTING_ENGINE_H
