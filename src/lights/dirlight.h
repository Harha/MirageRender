#ifndef DIRLIGHT_H
#define DIRLIGHT_H

// std includes

// mirage includes
#include "../core/light.h"

namespace mirage
{

class DirectionalLight : public virtual Light
{
public:
    DirectionalLight(Transform l2w = Transform(), vec3 emission = vec3(1, 1, 1), float distance = 1028);
    virtual void Le(const vec3 &P, const vec3 &N, const vec3 &Wi, const vec3 &Wo, vec3 &Li) const override;
    virtual void evalWe(const vec3 &P, const vec3 &N, const vec3 &Wo, vec3 &We) const override;
private:
    float m_distance;
};

}

#endif // DIRLIGHT_H
