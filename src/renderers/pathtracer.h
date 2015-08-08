#ifndef PATHTRACER_H
#define PATHTRACER_H

// std includes

// mirage includes
#include "../core/renderer.h"
#include "../math/math.h"
#include "../math/vec3.h"

namespace mirage
{

class Pathtracer : public Renderer
{
public:
    Pathtracer(int maxRecursion = 1);
    virtual void render(const Scene *scene, Display *display) override;
    vec3 radiance(const Scene *scene, const Ray &r, int n);
private:
    int m_maxRecursion;
};

}

#endif // PATHTRACER_H
