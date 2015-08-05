// mirage includes
#include "ray.h"

namespace mirage
{

Ray::Ray(vec3 origin, vec3 direction, float tmin, float tmax) : m_origin(origin), m_direction(direction.normalize()), m_directionInv(1.0f / m_direction), mint(tmin), maxt(tmax)
{

}

void Ray::setOrigin(const vec3 &origin)
{
    m_origin = origin;
}

void Ray::setDirection(const vec3 &direction)
{
    m_direction = direction.normalize();
    m_directionInv.x = 1.0f / m_direction.x;
    m_directionInv.y = 1.0f / m_direction.y;
    m_directionInv.z = 1.0f / m_direction.z;
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

/*
float Ray::getMinT() const
{
    return m_tmin;
}

float Ray::getMaxT() const
{
    return m_tmax;
}
*/

vec3 Ray::operator()(float t) const
{
    return m_origin + m_direction * t;
}

}
