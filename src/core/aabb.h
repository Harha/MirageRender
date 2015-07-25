#ifndef AABB_H
#define AABB_H

// mirage includes
#include "../math/vec3.h"

namespace mirage
{

class AABB
{
public:
    AABB(vec3 p1 = vec3(-1, -1, -1), vec3 p2 = vec3(1, 1, 1));
    AABB addPoint(const vec3 &p) const;
    AABB addBox(const AABB &b) const;
    AABB expand(const float delta) const;
    bool overlaps(const AABB &b) const;
    bool inside(const vec3 &p) const;
    float getSurfaceArea() const;
    float getVolume() const;
    int getMaximumExtent() const;
private:
    vec3 m_pmin;
    vec3 m_pmax;
};

}

#endif // AABB_H
