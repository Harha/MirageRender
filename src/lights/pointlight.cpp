// std includes
#include <iostream>

// mirage includes
#include "pointlight.h"
#include "../macros.h"

namespace mirage
{

PointLight::PointLight(Transform l2w, vec3 emission, float aC, float aL, float aQ) : Light(l2w, emission), m_attenuationC(aC), m_attenuationL(aL), m_attenuationQ(aQ)
{
    LOG("PointLight: Created a new point light source.");
}

void PointLight::Le(const vec3 &P, const vec3 &N, const vec3 &Wi, const vec3 &Wo, vec3 &Le) const
{
    // Calculate distance from light to P
    float d = Wi.length();

    // Get light attenuatin factor
    float attenuation = 1.0f / (m_attenuationC + m_attenuationL * d + m_attenuationQ * d * d);

    // Get incident radiation
    Le = m_emission * attenuation;
}

void PointLight::evalWe(const vec3 &P, const vec3 &N, const vec3 &Wo, vec3 &We) const
{
    We = m_lightToWorld.getPosition() - P;
}

}
