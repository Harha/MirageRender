#ifndef LIGHT_H
#define LIGHT_H

// std includes

// mirage includes
#include "transform.h"
#include "../math/vec3.h"

namespace mirage
{

class Light
{
public:
    Light(Transform l2w = Transform(), vec3 emission = vec3(1, 1, 1));
    virtual ~Light();
    virtual void Le(const vec3 &P, const vec3 &N, const vec3 &Wi, const vec3 &Wo, vec3 &Le) const = 0;
    virtual void evalWe(const vec3 &P, const vec3 &N, const vec3 &Wo, vec3 &We) const = 0;
    Transform getL2W() const;
private:
protected:
    Transform m_lightToWorld;
    vec3 m_emission;
};

}

#endif // LIGHT_H
