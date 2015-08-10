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
    virtual void render(const Scene *scene, Display *display, const int w, const int h, const int xa, const int ya) = 0;
private:
protected:
};

}

#endif // RENDERER_H
