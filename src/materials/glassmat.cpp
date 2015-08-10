// std includes
#include <iostream>

// mirage includes
#include "glassmat.h"
#include "../macros.h"

namespace mirage
{

GlassMaterial::GlassMaterial(vec3 kd, vec3 ks, vec3 ke) : Material(kd, ks, ke)
{

}

void GlassMaterial::evalBRDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &brdf) const
{

}

void GlassMaterial::evalBTDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &btdf) const
{

}

void GlassMaterial::evalPDF(float &pdf) const
{
    pdf = 1.0f;
}

void GlassMaterial::evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wi)
{
    Wi = vec3::reflect(Wo.negate(), N);
}

}
