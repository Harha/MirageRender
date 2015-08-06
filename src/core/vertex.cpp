// std includes

// mirage includes
#include "vertex.h"

namespace mirage
{

Vertex::Vertex(vec3 p, vec3 n) : m_p(p), m_n(n)
{

}

void Vertex::setPosition(const vec3 &v)
{
    m_p = v;
}

void Vertex::setNormal(const vec3 &n)
{
    m_n = n;
}

vec3 Vertex::getPosition() const
{
    return m_p;
}

vec3 Vertex::getNormal() const
{
    return m_n;
}

}
