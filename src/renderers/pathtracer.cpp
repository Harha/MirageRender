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

void Pathtracer::render(const Scene *scene, Display *display, const int w, const int h, const int xa, const int ya)
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

            // Add the radiance to film sample
            film->addSample(i, j, lambda);

            // Update the pixel on screen
            display->setPixel(i, j, film->getSample(i, j).getColorAveraged());
        }
    }
}

vec3 Pathtracer::radiance(const Scene *scene, const Ray &ray, float weight, int n)
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
        return weight * m_ka;
    }

    // Get the Kd, Ks & Ke surface data
    Material *M = iSect.getMaterial();
    vec3 Kd = M->getKd();
    vec3 Ks = M->getKs();
    vec3 Ke = M->getKe();

    // Find the maximum reflectance amount
    float Kd_max = Kd.x > Kd.y && Kd.x > Kd.z ? Kd.x : Kd.y > Kd.z ? Kd.y : Kd.z;

    // Find the XYZ color space luminance of the surface
    //float Y = 0.2126 * Kd.x + 0.7152 * Kd.y + 0.0722 * Kd.z;

    // Russian roulette, absorb or continue
    float p = pseudorand();
    p = (p != 0.0f) ? p : 0.01f;
    p = (p != 1.0f) ? p : 0.99f;
    if (weight < p)
    {
        return (1.0f / p) * Ke;
    }

    // Assign intersection data into aliases
    vec3 Wo = ray.getDirection().negate();
    vec3 P = iSect.getPosition();
    vec3 N = iSect.getNormal();

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
    vec3 Le, Ler, Let;
    std::vector<Light *> lights = scene->getLights();
    for (size_t i = 0; i < lights.size(); i++)
    {
        // Get the current light in list with i
        Light *currlight = lights[i];

        // Calculate the We vector from P to L
        vec3 We;
        currlight->evalWe(P, N, Wo, We);

        // Generate a shadow ray
        Ray r_shadow(P, We, 0.0f, We.length() - EPSILON);

        // Intersect the scene with it and add light contribution to Le if nothing was hit
        if (!scene->intersectP(r_shadow))
        {
            // Get the surface brdf & btdf
            M->evalBSDF_direct(P, N, We.normalize(), Wr, Wt, Wo, BRDF_direct, BTDF_direct);

            // Create a temporary variable to hold the current Le
            vec3 Ler_, Let_;

            // Get the light amount from We
            currlight->Le(P, N, We, Wo, Ler_); // Parameters are silly, gotta change this

            // Scale the current Le_ by BRDF_direct
            Ler_ *= BRDF_direct;
            Let_ *= BTDF_direct;

            // Add the Le_'s contribution to the total Le
            Le += Ler_ + Let_;
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
    return (1.0f / (1.0f - p)) * Ke + Kd * (weight * Le + (BRDF / PDF) * Lr + (BTDF / PDF) * Lt);
}

}
