#ifndef POINTLIGHT_H
#define POINTLIGHT_H

// std includes

// mirage includes
#include "../core/light.h"

namespace mirage
{

class PointLight : public virtual Light
{
public:
    PointLight(Transform l2w = Transform(), vec3 emission = vec3(1, 1, 1), float aC = 0, float aL = 1, float aQ = 0);
    virtual void Le(const vec3 &P, const vec3 &N, const vec3 &Wi, const vec3 &Wo, vec3 &Li) const override;
    virtual void evalWe(const vec3 &P, const vec3 &N, const vec3 &Wo, vec3 &We) const override;
private:
    float m_attenuationC;
    float m_attenuationL;
    float m_attenuationQ;
};

}

#endif // POINTLIGHT_H
