// std includes

// mirage includes
#include "pathtracer.h"
#include "../core/intersection.h"

namespace mirage
{

static vec3 COLOR_NULL(0, 0, 0);
static vec3 COLOR_AMBI = vec3(0.75f, 0.87f, 0.98f) * 0.1f;

Pathtracer::Pathtracer(int maxRecursion) : m_maxRecursion(maxRecursion)
{

}

void Pathtracer::render(const Scene *scene, Display *display)
{
    Camera *camera = scene->getCamera();
    Film *film = &camera->getFilm();
    Ray r_primary;

    for (size_t j = 0; j < film->getResolutionY(); j++)
    {
        for (size_t i = 0; i < film->getResolutionX(); i++)
        {
            // Project the primary ray through the camera's lens
            camera->calcCamRay(i, j, r_primary);

            // Let's find the final radiance along the ray
            vec3 lambda = radiance(scene, r_primary, 0);

            // Add the radiance to film sample
            film->addSample(i, j, lambda);

            // Update the pixel on screen
            display->setPixel(i, j, film->getSample(i, j).getColorAveraged());
        }
    }
}

vec3 Pathtracer::radiance(const Scene *scene, const Ray &ray, int n)
{
    // Return if recursion limit was reached
    if (n > m_maxRecursion)
    {
        return COLOR_NULL;
    }

    // Find the closest intersection
    Intersection iSect;
    if (!scene->getAccelerator()->intersect(ray, iSect))
    {
        return COLOR_AMBI;
    }

    // Return immediately with the emittance if the surface is ONLY emissive
    if (iSect.getMaterial().getKe().length() > 0.0f && iSect.getMaterial().getKd().length() <= 0.0f)
    {
        return iSect.getMaterial().getKe();
    }

    // Assign intersection data into aliases
    auto V_vector = ray.getDirection();
    auto P_vector = iSect.getPosition();
    auto N_vector = iSect.getNormal();
    auto M_info = iSect.getMaterial();
    auto e = M_info.getKe();
    auto f = M_info.getKd();

    // Russian roulette to stop recursion randomly...
    auto p = f.x > f.y && f.x > f.z ? f.x : f.y > f.z ? f.y : f.z;
    if (n > m_maxRecursion/2 || !p)
        if (pseudorand() < p)
            f *= (1.0f / p);

    // Calculate the light direction ray
    auto L_rand = vec3::sampleHemisphere(N_vector);

    // Prepare all required information
    auto L_vector = L_rand.negate();

    // Calculate all dot products
    auto NdotL = std::abs(vec3::dot(N_vector, L_vector));

    // Calculate the bidirectional reflectance function value
    auto BRDF = (f / PI) * NdotL;

    // Calculate the probability density function value (1.0f / PI)
    auto PDF = PI_1;

    // Get the reflected light amount from L_rand
    auto REFL = radiance(scene, Ray(P_vector, L_rand), n + 1);

    // Return the final radiance
    return e + BRDF / PDF * REFL;
}

}
