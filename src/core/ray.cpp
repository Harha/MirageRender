// mirage includes
#include "ray.h"

namespace mirage
{

Ray::Ray(vec3 origin, vec3 direction, float tmin, float tmax) : m_origin(origin), m_direction(direction.normalize()), m_directionInv(1.0f / m_direction), m_tmin(tmin), m_tmax(tmax)
{

}

void Ray::setOrigin(const vec3 &origin)
{
    m_origin = origin;
}

void Ray::setDirection(const vec3 &direction)
{
    m_direction = direction.normalize();
    m_directionInv = 1.0f / direction;
}

vec3 Ray::getOrigin() const
{
    return m_origin;
}

vec3 Ray::getDirection() const
{
    return m_direction;
}

vec3 Ray::getDirectionInv() const
{
    return m_directionInv;
}

vec3 Ray::operator()(float t) const
{
    return m_origin + m_direction * t;
}

}
