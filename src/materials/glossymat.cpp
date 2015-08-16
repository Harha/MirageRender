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

GlossyMaterial::~GlossyMaterial()
{

}

void GlossyMaterial::evalBSDF(const vec3 &P, const vec3 &N, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const
{
    // Get the surface values
    float R = m_r + EPSILON;
    float RR = R * R;
    float F = m_k;
    float K = m_d;

    // Prepare all required information
    vec3 V_vector = Wo;
    vec3 N_vector = N;
    vec3 L_vector = Wr;
    vec3 H_vector = (V_vector + L_vector).normalize();

    // Calculate all dot products
    float NdotL = vec3::dot(N_vector, L_vector);
    float NdotV = vec3::dot(N_vector, V_vector);
    float NdotH = vec3::dot(N_vector, H_vector);
    float VdotH = vec3::dot(V_vector, H_vector);

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

    // There's no transmission, so btdf stays 0.0f
    btdf = 0.0f;
}

void GlossyMaterial::evalBSDF_direct(const vec3 &P, const vec3 &N, const vec3 &We, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const
{
    evalBSDF(P, N, We, Wt, Wo, brdf, btdf);
}

void GlossyMaterial::evalPDF(float &pdf) const
{
    pdf = 1.0f / (2.0f * PI * m_r);
}

void GlossyMaterial::evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt)
{
    // Calculate mirror / random reflection vectors
    vec3 Wr_mirr = vec3::reflect(Wo.negate(), N).normalize();
    vec3 Wr_rand = vec3::sampleHemisphere(Wr_mirr, m_r, 1.0f - m_r);

    // Make sure the random ray doesn't go through surface
    if (vec3::dot(N, Wr_rand) < 0.0f)
    {
        Wr_rand = vec3::reflect(Wr_rand, N);
    }

    // Assign the reflected ray to Wr
    Wr = Wr_rand.normalize();

    // No transmission, so Wt stays 0.0f
    Wt = vec3();
}

}
