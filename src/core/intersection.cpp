// mirage includes
#include "intersection.h"

namespace mirage
{

Intersection::Intersection(float t, vec3 position, vec3 normal, Shape shape) : m_t(t), m_position(position), m_normal(normal), m_shape(shape)
{

}

void Intersection::setT(const float t)
{

}

void Intersection::setPosition(const vec3 &v)
{

}

void Intersection::setNormal(const vec3 &v)
{

}

void Intersection::setShape(const Shape &s)
{

}

float Intersection::getT() const
{
    return m_t;
}

vec3 Intersection::getPosition() const
{
    return m_position;
}

vec3 Intersection::getNormal() const
{
    return m_normal;
}

Shape Intersection::getShape() const
{
    return m_shape;
}

}
