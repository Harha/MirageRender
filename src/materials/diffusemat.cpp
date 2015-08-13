// std includes
#include <iostream>

// mirage includes
#include "diffusemat.h"
#include "../macros.h"

namespace mirage
{

DiffuseMaterial::DiffuseMaterial(vec3 kd, vec3 ke) : Material(kd, 0.0f, ke)
{

}

DiffuseMaterial::~DiffuseMaterial()
{

}

void DiffuseMaterial::evalBRDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &brdf) const
{
    // Calculate the cosi term
    float cosi = vec3::dot(Wi, n);

    // Calculate the lambertian brdf
    brdf = PI_INV * cosi;

    if (brdf > 1.0f / PI || brdf < 0.0f)
        ERR("ERROR! BRDF OVER 1 / PI OR NEGATIVE! BRDF: " << brdf);
}

void DiffuseMaterial::evalBTDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &btdf) const
{
    btdf = 0.0f;
}

void DiffuseMaterial::evalPDF(float &pdf) const
{
    pdf = 1.0f / (2.0f * PI);
}

void DiffuseMaterial::evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt)
{
    Wr = vec3::sampleHemisphere(N);
}

}
