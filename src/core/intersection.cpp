// mirage includes
#include "intersection.h"

namespace mirage
{

Intersection::Intersection(vec3 position, vec3 normal, Material *material) : m_position(position), m_normal(normal), m_material(material)
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

void Intersection::setMaterial(Material *m)
{
    m_material = m;
}

vec3 Intersection::getPosition() const
{
    return m_position;
}

vec3 Intersection::getNormal() const
{
    return m_normal;
}

Material *Intersection::getMaterial() const
{
    return m_material;
}

}
