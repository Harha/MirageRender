// std includes
#include <algorithm>

// mirage includes
#include "aabb.h"

namespace mirage
{

AABB::AABB(vec3 p1, vec3 p2) : m_pmin(p1), m_pmax(p2)
{
    m_pmin = vec3(std::min(p1.x, p2.x), std::min(p1.y, p2.y), std::min(p1.z, p2.z));
    m_pmax = vec3(std::max(p1.x, p2.x), std::max(p1.y, p2.y), std::max(p1.z, p2.z));
}

AABB AABB::addPoint(const vec3 &p) const
{
    AABB result;

    result.m_pmin.x = std::min(m_pmin.x, p.x);
    result.m_pmin.y = std::min(m_pmin.y, p.y);
    result.m_pmin.z = std::min(m_pmin.z, p.z);
    result.m_pmax.x = std::max(m_pmax.x, p.x);
    result.m_pmax.y = std::max(m_pmax.y, p.y);
    result.m_pmax.z = std::max(m_pmax.z, p.z);

    return result;
}

AABB AABB::addBox(const AABB &b) const
{
    AABB result;

    result.m_pmin.x = std::min(m_pmin.x, b.m_pmin.x);
    result.m_pmin.y = std::min(m_pmin.y, b.m_pmin.y);
    result.m_pmin.z = std::min(m_pmin.z, b.m_pmin.z);
    result.m_pmax.x = std::max(m_pmax.x, b.m_pmax.x);
    result.m_pmax.y = std::max(m_pmax.y, b.m_pmax.y);
    result.m_pmax.z = std::max(m_pmax.z, b.m_pmax.z);

    return result;
}

AABB AABB::expand(const float delta) const
{
    AABB result(m_pmin, m_pmax);

    result.m_pmin -= delta;
    result.m_pmax += delta;

    return result;
}

bool AABB::overlaps(const AABB &b) const
{
    bool x = (m_pmax.x >= b.m_pmin.x) && (m_pmin.x <= b.m_pmax.x);
    bool y = (m_pmax.y >= b.m_pmin.y) && (m_pmin.y <= b.m_pmax.y);
    bool z = (m_pmax.z >= b.m_pmin.z) && (m_pmin.z <= b.m_pmax.z);

    return (x && y && z);
}

bool AABB::inside(const vec3 &p) const
{
    return (p.x >= m_pmin.x && p.x <= m_pmax.x &&
            p.y >= m_pmin.y && p.y <= m_pmax.y &&
            p.z >= m_pmin.z && p.z <= m_pmax.z);
}

float AABB::getSurfaceArea() const
{
    vec3 d = m_pmax - m_pmin;
    return 2.0f * (d.x * d.y + d.x * d.z + d.y * d.z);
}

float AABB::getVolume() const
{
    vec3 d = m_pmax - m_pmin;
    return d.x * d.y * d.z;
}

int AABB::getMaximumExtent() const
{
    vec3 d = m_pmax - m_pmin;
    if (d.x > d.y && d.x > d.z)
    {
        return 0;
    }
    else if (d.y > d.z)
    {
        return 1;
    }
    else
    {
        return 2;
    }
}



}
