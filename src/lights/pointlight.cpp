// std includes

// mirage includes
#include "pointlight.h"

namespace mirage
{

PointLight::PointLight(Transform l2w, vec3 emission, float aC, float aL, float aQ) : Light(l2w, emission), m_attenuationC(aC), m_attenuationL(aL), m_attenuationQ(aQ)
{

}

void PointLight::Le(const vec3 &P, const vec3 &N, const vec3 &Wi, const vec3 &Wo, vec3 &Le) const
{
    // Calculate distance from light to P
    float d = Wi.length();

    // Get light attenuatin factor
    float attenuation = m_attenuationC + m_attenuationL * d + m_attenuationQ * d * d;

    // Get incident radiation
    Le = m_emission / attenuation;
}

void PointLight::evalWe(const vec3 &P, const vec3 &N, const vec3 &Wo, vec3 &We) const
{
    // Calculate the P -> L vector
    We = m_lightToWorld.getPosition() - P;
}

}
