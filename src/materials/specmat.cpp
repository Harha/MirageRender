// std includes
#include <iostream>

// mirage includes
#include "specmat.h"
#include "../macros.h"

namespace mirage
{

SpecularMaterial::SpecularMaterial(vec3 kd, vec3 ks, vec3 ke) : Material(kd, ks, ke)
{

}

SpecularMaterial::~SpecularMaterial()
{

}

void SpecularMaterial::evalBRDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &brdf) const
{
    brdf = 1.0f;
}

void SpecularMaterial::evalBTDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &btdf) const
{
    btdf = 0.0f;
}

void SpecularMaterial::evalPDF(float &pdf) const
{
    pdf = 1.0f;
}

void SpecularMaterial::evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt)
{
    Wr = vec3::reflect(Wo.negate(), N);
}

}
