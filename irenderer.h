#ifndef IRENDERER_H
#define IRENDERER_H

namespace core
{

class IRenderer
{
public:
    virtual void resize(int width, int height) = 0;
    virtual void init() = 0;
    virtual void draw() = 0;

    ~IRenderer() {}
};

} // namespace core

#endif // IRENDERER_H
