// mirage includes
#include "ray.h"

namespace mirage
{

Ray::Ray(vec4 origin, vec4 direction) : m_origin(origin), m_direction(direction), m_directionInv(1.0f / direction)
{

}

void Ray::setOrigin(const vec4 &origin)
{
    m_origin = origin;
}

void Ray::setDirection(const vec4 &direction)
{
    m_direction = direction;
    m_directionInv = 1.0f / direction;
}

vec4 Ray::getOrigin() const
{
    return m_origin;
}

vec4 Ray::getDirection() const
{
    return m_direction;
}

vec4 Ray::getDirectionInv() const
{
    return m_directionInv;
}

}
