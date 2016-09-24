#ifndef PATHTRACER_H
#define PATHTRACER_H

// std includes

// mirage includes
#include "../core/renderer.h"
#include "../math/math.h"
#include "../math/vec3.h"

namespace mirage
{

class Pathtracer : public virtual Renderer
{
public:
    Pathtracer(float maxRadiance = 10.0f, int maxRecursion = 1);
    virtual void render(const Scene * const scene, Display * const display, const unsigned w, const unsigned h, const unsigned xa, const unsigned ya) override;
    vec3 radiance(const Scene * const scene, const Ray &ray, const float weight, const int n);
private:
    float m_maxRadiance;
    int m_maxRecursion;
};

}

#endif // PATHTRACER_H
