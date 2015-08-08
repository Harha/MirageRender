#ifndef RENDERER_H
#define RENDERER_H

// std includes

// mirage includes
#include "scene.h"
#include "display.h"


namespace mirage
{

class Renderer
{
public:
    virtual void render(const Scene *scene, Display *display) = 0;
private:
protected:
};

}

#endif // RENDERER_H
