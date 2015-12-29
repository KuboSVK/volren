#include "ray_casting_engine.h"

namespace core
{

RayCastingEngine::RayCastingEngine()
: mWidth(800)
, mHeight(600)
{

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

} // namespace core
