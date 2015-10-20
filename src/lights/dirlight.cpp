// std includes
#include <iostream>

// mirage includes
#include "dirlight.h"
#include "../macros.h"

namespace mirage
{

DirectionalLight::DirectionalLight(Transform l2w, vec3 emission, float distance) : Light(l2w, emission), m_distance(distance)
{
    LOG("DirectionalLight: Created a new directional light source.");
}

void DirectionalLight::Le(const vec3 &P, const vec3 &N, const vec3 &Wi, const vec3 &Wo, vec3 &Le) const
{
    Le = m_emission;
}

void DirectionalLight::evalWe(const vec3 &P, const vec3 &N, const vec3 &Wo, vec3 &We) const
{
    We = m_lightToWorld.getOrientation().getForwardVector().negate() * m_distance;
}

}
