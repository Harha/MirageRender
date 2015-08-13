// std includes
#include <iostream>

// mirage includes
#include "glassmat.h"
#include "../macros.h"

namespace mirage
{

GlassMaterial::GlassMaterial(vec3 kd, vec3 ks, vec3 ke, float ior) : Material(kd, ks, ke), m_ior(ior)
{

}

GlassMaterial::~GlassMaterial()
{

}

void GlassMaterial::evalBRDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &brdf) const
{
    // Are we going into the medium or out of it?
    vec3 Nt = vec3::dot(n, Wo) > 0.0f ? n : n.negate();
    bool into = vec3::dot(n, Nt) > 0.0f;

    // Calculate the dot product between Wi & Nt
    float WidotNt = vec3::dot(Wo, Nt);

    // Get the right indices of refraction
    float n1 = into ? 1.0f : m_ior;
    float n2 = into ? m_ior : 1.0f;

    // Calculate the brdf (schlick's approx)
    float R0 = (n1 - n2 / (n1 + n2)) * (n1 - n2 / (n1 + n2));
    brdf = R0 + (1.0f - R0) * std::pow((1.0f - WidotNt), 5.0f);
}

void GlassMaterial::evalBTDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &btdf) const
{
    // Are we going into the medium or out of it?
    vec3 Nt = vec3::dot(n, Wo) > 0.0f ? n : n.negate();
    bool into = vec3::dot(n, Nt) > 0.0f;

    // Calculate the dot products
    float WidotNt = vec3::dot(Wi.negate(), Nt);
    float WodotNt = vec3::dot(Wo, Nt);

    // Get the right indices of refraction
    float n1 = into ? m_ior : 1.0f;
    float n2 = into ? 1.0f : m_ior;

    // Calculate the btdf
    btdf = (n2 * WidotNt) / (n1 * WodotNt);
}

void GlassMaterial::evalPDF(float &pdf) const
{
    pdf = 1.0f;
}

void GlassMaterial::evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt)
{
    // Are we going into the medium or out of it?
    vec3 n = vec3::dot(N, Wo) > 0.0f ? N : N.negate();
    bool into = vec3::dot(N, n) > 0.0f;

    // Calculate the reflected ray
    Wr = vec3::reflect(Wo.negate(), n);

    // Calculate the ratio of indices of refraction accordingly
    float eta = into ? 1.0f / m_ior : m_ior;

    // Refract the ray through the surface
    Wt = vec3::refract(Wo.negate(), n, eta);

    // If total internal reflection angle is exceeded, reflect instead
    if (Wt.length() == 0.0f)
    {
        Wt = vec3(Wr.x, Wr.y, Wr.z);
    }
}

}
