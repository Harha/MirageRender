// std includes
#include <iostream>

// mirage includes
#include "raytracer.h"
#include "../macros.h"
#include "../core/intersection.h"

namespace mirage
{

static vec3 COLOR_NULL(0, 0, 0);

Raytracer::Raytracer(vec3 ka, float maxRadiance, int maxRecursion) : m_ka(ka), m_maxRadiance(maxRadiance), m_maxRecursion(maxRecursion)
{

}

void Raytracer::render(const Scene *scene, Display *display, const int w, const int h, const int xa, const int ya)
{
    Camera *camera = scene->getCamera();
    Film *film = &camera->getFilm();
    Ray r_primary;

    for (size_t j = ya; j < ya + h; j++)
    {
        for (size_t i = xa; i < xa + w; i++)
        {
            // Project the primary ray through the camera's lens
            camera->calcCamRay(i, j, r_primary);

            // Let's find the final radiance along the ray
            vec3 lambda = radiance(scene, r_primary, 1.0f, 0);

            // Set the radiance to film sample
            film->setSample(i, j, lambda);

            // Update the pixel on screen
            display->setPixel(i, j, film->getSample(i, j).getColor());
        }
    }
}

vec3 Raytracer::radiance(const Scene *scene, const Ray &ray, float weight, int n)
{
    // Return if recursion limit was reached
    if (n > m_maxRecursion || weight <= 0.0f)
    {
        return COLOR_NULL;
    }

    // Find the closest intersection
    Intersection iSect;
    if (!scene->getAccelerator()->intersect(ray, iSect))
    {
        return weight * m_ka;
    }

    // Get the Kd, Ks & Ke surface data
    Material *M = iSect.getMaterial();
    vec3 Kd = M->getKd();
    vec3 Ks = iSect.getMaterial()->getKs();
    vec3 Ke = iSect.getMaterial()->getKe();

    // Assign intersection data into aliases
    vec3 Wo = ray.getDirection().negate();
    vec3 P = iSect.getPosition();
    vec3 N = iSect.getNormal();

    // Get the Wr & Wt vectors (reflected & transmitted)
    vec3 Wr = vec3::reflect(Wo.negate(), N);
    vec3 Wt = vec3::refract(Wo.negate(), N, 1.0f);

    // Get the surface brdf
    float BRDF;
    M->evalBRDF(P, N, Wr, Wo, BRDF);

    // Get the surface btdf
    /*
    float BTDF;
    M->evalBTDF(P, N, Wt, Wo, BTDF);

    // Get the light amount from Wr
    vec3 Lr;
    if (Wr.length() > 0.0f)
    {
        Lr = radiance(scene, Ray(P, Wr), weight, n + 1);
    }

    // Get the light amount from Wt
    vec3 Lt;
    if (Wt.length() > 0.0f)
    {
        Lt = radiance(scene, Ray(P, Wt), weight, n + 1);
    }*/

    // Return the final radiance
    return weight * Kd * BRDF;
}

}
