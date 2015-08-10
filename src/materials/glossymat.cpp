// std includes
#include <iostream>

// mirage includes
#include "glossymat.h"
#include "../macros.h"

namespace mirage
{

GlossyMaterial::GlossyMaterial(vec3 kd, vec3 ks, vec3 ke, float r, float k, float d) : Material(kd, ks, ke), m_r(r), m_k(k), m_d(d)
{

}

void GlossyMaterial::evalBRDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &brdf) const
{
    // Get the surface values
    float R = m_r + EPSILON;
    float RR = R * R;
    float F = m_k;
    float K = m_d;

    // Prepare all required information
    auto V_vector = Wo;
    auto N_vector = n;
    auto L_vector = Wi;
    auto H_vector = (V_vector + L_vector).normalize();

    // Calculate all dot products
    auto NdotL = std::abs(vec3::dot(N_vector, L_vector));
    auto NdotV = std::abs(vec3::dot(N_vector, V_vector));
    auto NdotH = std::abs(vec3::dot(N_vector, H_vector));
    auto VdotH = std::abs(vec3::dot(V_vector, H_vector));

    // Evaluate the geometric term
    float geo_numer = 2.0f * NdotH;
    float geo_denom = VdotH;
    float geo = std::min(1.0f, std::min((geo_numer * NdotV) / geo_denom, (geo_numer * NdotL) / geo_denom));

    // Evaluate the roughness term
    float rough_a = 1.0f / (4.0f * RR * std::pow(NdotH, 4.0f));
    float rough_b = NdotH * NdotH - 1.0f;
    float rough_c = RR * NdotH * NdotH;
    float rough = rough_a * std::exp(rough_b / rough_c);

    // Evaluate the fresnel term
    float fresnel = std::pow(1.0f - VdotH, 5.0f);
    fresnel *= 1.0f - F;
    fresnel += F;

    // Put the terms together
    float Rs = (geo * rough * fresnel) / (PI * NdotV * NdotL + EPSILON);

    // Calculate the cook-torrance brdf value
    brdf = (1.0f / PI) * NdotL * (Rs * (1.0f - K) + K);

    if (brdf < 0.0f)
        ERR("ERROR! BRDF NEGATIVE! BRDF: " << brdf);
}

void GlossyMaterial::evalBTDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &btdf) const
{

}

void GlossyMaterial::evalPDF(float &pdf) const
{
    pdf = 1.0f / (2.0f * PI * m_r);
}

void GlossyMaterial::evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wi)
{
    // Calculate mirror / random reflection vectors
    vec3 Wi_mirr = vec3::reflect(Wo.negate(), N).normalize();
    vec3 Wi_rand = vec3::sampleHemisphere(Wi_mirr, m_r, 0.33f);

    // Make sure the random ray doesn't go through surface
    if (vec3::dot(N, Wi_rand) < 0.0f)
    {
        Wi_rand = vec3::reflect(Wi_rand, N);
    }

    Wi = Wi_rand.normalize();
}

}
