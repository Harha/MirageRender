// mirage includes
#include "intersection.h"

namespace mirage
{

Intersection::Intersection(vec3 position, vec3 normal) : m_position(position), m_normal(normal)
{

}

void Intersection::setPosition(const vec3 &v)
{
    m_position = v;
}

void Intersection::setNormal(const vec3 &v)
{
    m_normal = v;
}

vec3 Intersection::getPosition() const
{
    return m_position;
}

vec3 Intersection::getNormal() const
{
    return m_normal;
}

}
