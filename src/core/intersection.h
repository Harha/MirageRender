#ifndef INTERSECTION_H
#define INTERSECTION_H

// mirage includes
#include "../math/vec3.h"

namespace mirage
{

class Intersection
{
public:
    Intersection(vec3 position = vec3(), vec3 normal = vec3());
    void setPosition(const vec3 &v);
    void setNormal(const vec3 &v);
    vec3 getPosition() const;
    vec3 getNormal() const;
private:
    vec3 m_position;
    vec3 m_normal;
};

}

#endif // INTERSECTION_H
