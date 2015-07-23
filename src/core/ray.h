#ifndef RAY_H
#define RAY_H

// std includes

// mirage includes
#include "../math/vec4.h"

namespace mirage
{

class Ray
{
public:
    Ray(vec4 origin = vec4(), vec4 direction = vec4()) : m_origin(origin), m_direction(direction), m_directionInv(1.0f / direction) { }

    void setOrigin(const vec4 &origin)
    {
        m_origin = origin;
    }

    void setDirection(const vec4 &direction)
    {
        m_direction = direction;
        m_directionInv = 1.0f / direction;
    }

    vec4 getOrigin() const
    {
        return m_origin;
    }

    vec4 getDirection() const
    {
        return m_direction;
    }

    vec4 getDirectionInv() const
    {
        return m_directionInv;
    }

private:
    vec4 m_origin;
    vec4 m_direction;
    vec4 m_directionInv;
};

}

#endif // RAY_H
