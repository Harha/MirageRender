// std includes
#include <iostream>

// mirage includes
#include "pathtracer.h"
#include "../macros.h"
#include "../core/intersection.h"

namespace mirage
{

static vec3 COLOR_NULL(0, 0, 0);

Pathtracer::Pathtracer(vec3 ka, float maxRadiance, int maxRecursion) : m_ka(ka), m_maxRadiance(maxRadiance), m_maxRecursion(maxRecursion)
{

}

void Pathtracer::render(const Scene * const scene, Display * const display, const unsigned w, const unsigned h, const unsigned xa, const unsigned ya)
{
    Camera *const camera = scene->getCamera();
    Film * const film = &camera->getFilm();
    Ray r_primary;

    for (size_t j = ya; j < ya + h; j++)
    {
        for (size_t i = xa; i < xa + w; i++)
        {
            // Project the primary ray through the camera's lens
            camera->calcCamRay(i, j, r_primary);

            // Let's find the final radiance along the ray
            vec3 lambda = radiance(scene, r_primary, 1.0f, 0);

            // Add the radiance to film sample
            film->addSample(i, j, lambda);

            // Update the pixel on screen
            display->setPixel(i, j, film->getSample(i, j).getColorAveraged());
        }
    }
}

vec3 Pathtracer::radiance(const Scene * const scene, const Ray &ray, const float weight, const int n)
{
    // Return if recursion limit was reached
    if (n >= m_maxRecursion || weight <= 0.0f)
    {
        return COLOR_NULL;
    }

    // Find the closest intersection
    Intersection iSect;
    if (!scene->getAccelerator()->intersect(ray, iSect))
    {
        return (m_ka.x == 0.0f && m_ka.y == 0.0f && m_ka.z == 0.0f) ? COLOR_NULL : weight * m_ka;
    }

    // Get the Kd, Ks & Ke surface data
    Material * const M = iSect.getMaterial();
    const vec3 &Kd = M->getKd();
    //const vec3 &Ks = M->getKs();
    const vec3 &Ke = M->getKe();

    // Find the maximum reflectance amount
    const float Kd_max = Kd.x > Kd.y && Kd.x > Kd.z ? Kd.x : Kd.y > Kd.z ? Kd.y : Kd.z;

    // Russian roulette, absorb or continue
    float p = pseudorand();
    p = (p != 0.0f) ? p : 0.0001f;
    p = (p != 1.0f) ? p : 0.9999f;
    if (weight < p)
    {
        return (1.0f / p) * Ke;
    }

    // Assign intersection data into aliases
    const vec3 &Wo = ray.getDirection().negate();
    const vec3 &P = iSect.getPosition();
    const vec3 &N = iSect.getNormal();

    // Get the Wr & Wt vectors (reflected & transmitted)
    vec3 Wr;
    vec3 Wt;
    M->evalWi(Wo, N, Wr, Wt);

    // Get the surface brdf & btdf function output values
    float BRDF;
    float BRDF_direct;
    float BTDF = n;
    float BTDF_direct = n;
    M->evalBSDF(P, N, Wr, Wt, Wo, BRDF, BTDF);

    // Get the surface pdf
    float PDF;
    M->evalPDF(PDF);

    // Direct light sampling
    vec3 Le;
    const std::vector<Light *> &lights = scene->getLights();
    for (size_t i = 0; i < lights.size(); i++)
    {
        // Get the current light in list with i
        const Light * const currlight = lights[i];

        // Calculate the We vector from P to L
        vec3 We;
        currlight->evalWe(P, N, Wo, We);

        // Generate a shadow ray
        Ray r_shadow(P, We, 0.0f, We.length() - EPSILON);

        // Intersect the scene with it and add light contribution to Le if nothing was hit
        if (!scene->intersectP(r_shadow))
        {
            // Create a temporary variable to hold the current Le
            vec3 Ler;

            // Get the light amount from We
            currlight->Le(P, N, We, Wo, Ler);

            // Continue to next light if Ler_ is <= 0.0f
            if (Ler.length() <= 0.0f)
                continue;

            // Get the surface brdf & btdf
            M->evalBSDF_direct(P, N, We.normalize(), Wr, Wt, Wo, BRDF_direct, BTDF_direct);

            // Scale the current Le_ by BRDF_direct
            Ler *= BRDF_direct;

            // Add the Le_'s contribution to the total Le
            Le += Ler;
        }
    }

    // Get the light amount from Wr
    vec3 Lr;
    if (Wr.length() > 0.0f)
    {
        Lr = radiance(scene, Ray(P, Wr), weight * Kd_max, n + 1);
    }

    // Get the light amount from Wt
    vec3 Lt;
    if (Wt.length() > 0.0f)
    {
        Lt = radiance(scene, Ray(P, Wt), weight, n + 1);
    }

    // Return the final radiance
    return vec3::clampv((1.0f / (1.0f - p)) * Ke + Kd * (weight * Le + (BRDF / PDF) * Lr + (BTDF / PDF) * Lt), 0.0f, m_maxRadiance);
}

}
