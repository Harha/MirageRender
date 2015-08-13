#ifndef RAYTRACER_H
#define RAYTRACER_H

// std includes

// mirage includes
#include "../core/renderer.h"
#include "../math/math.h"
#include "../math/vec3.h"

namespace mirage
{

class Raytracer : public Renderer
{
public:
    Raytracer(vec3 ka = vec3(), float maxRadiance = 10.0f, int maxRecursion = 1);
    virtual void render(const Scene *scene, Display *display, const int w, const int h, const int xa, const int ya) override;
    vec3 radiance(const Scene *scene, const Ray &r, float weight, int n);
private:
    vec3 m_ka;
    float m_maxRadiance;
    int m_maxRecursion;
};

}

#endif // RAYTRACER_H
