// std includes
#include <iostream>

// mirage includes
#include "glassmat.h"
#include "../macros.h"

namespace mirage
{

GlassMaterial::GlassMaterial(vec3 kd, vec3 ks, vec3 ke, float ior) : Material(kd, ks, ke, true), m_ior(ior)
{

}

GlassMaterial::~GlassMaterial()
{

}

void GlassMaterial::evalBSDF(const vec3 &P, const vec3 &N, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const
{
    // Re-orient the normal, are we going into the medium or out of it?
    vec3 n = vec3::dot(N, Wo) > 0.0f ? N : N.negate();
    bool into = vec3::dot(N, n) > 0.0f;

    // Calculate the ratio of indices of refraction accordingly
    float nc = 1.0f;
    float eta = into ? nc / m_ior : m_ior;

    // Calculate some required variables for dielectric transmission / reflection
    float NdotWo = vec3::dot(n, Wo.negate());
    float cos2t = 1.0f - eta * eta * (1.0f - NdotWo * NdotWo);

    // Total internal reflection? (|Wt| = 0.0f) Handle accordingly, light is only reflected
    if (cos2t < 0.0f)
    {
        brdf = 1.0f;
        btdf = 0.0f;
        return;
    }

    // Choose reflection or refraction based on fresnel equations
    float a = m_ior - nc, b = m_ior + nc, R0 = a * a / (b * b), c = 1.0f - (into ? -NdotWo : vec3::dot(Wt, N));
    float Re = R0 + (1.0f - R0) * c * c * c * c * c, Tr = 1.0f - Re, P_ = 0.25f + 0.5f * Re, RP = Re / P_, TP = Tr / (1.0f - P_);

    // Assign brdf / btdf correct values
    if (btdf > 2)
    {
        float rand_float = pseudorand();
        brdf = (rand_float < P_) ? RP : 0.0f;
        btdf = (rand_float > P_) ? TP : 0.0f;
    }
    else
    {
        brdf = Re;
        btdf = Tr;
    }
}

void GlassMaterial::evalBSDF_direct(const vec3 &P, const vec3 &N, const vec3 &We, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const
{
    evalBSDF(P, N, We, Wt, Wo, brdf, btdf);
}

void GlassMaterial::evalPDF(float &pdf) const
{
    pdf = 1.0f;
}

void GlassMaterial::evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt)
{
    // Re-orient the normal, are we going into the medium or out of it?
    vec3 n = vec3::dot(N, Wo) > 0.0f ? N : N.negate();
    bool into = vec3::dot(N, n) > 0.0f;

    // Calculate the reflected ray
    Wr = vec3::reflect(Wo.negate(), N).normalize();

    // Calculate the ratio of indices of refraction accordingly
    float eta = into ? 1.0f / m_ior : m_ior;

    // Refract the ray through the surface, Wt becomes |0.0f| if total internal reflection
    Wt = vec3::refract(Wo.negate(), n, eta).normalize();
}

}
