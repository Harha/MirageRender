#ifndef AABB_H
#define AABB_H

// mirage includes
#include "../math/vec4.h"

namespace mirage
{

class AABB
{
public:
    AABB(vec4 p1 = vec4(-1, -1, -1, 1), vec4 p2 = vec4(1, 1, 1, 1));

    AABB addPoint(const vec4 &p) const;
    AABB addBox(const AABB &b) const;
    AABB expand(const float delta) const;
    bool overlaps(const AABB &b) const;
    bool inside(const vec4 &p) const;
    float getSurfaceArea() const;
    float getVolume() const;
    int getMaximumExtent() const;
private:
    vec4 m_pmin;
    vec4 m_pmax;
};

}

#endif // AABB_H
