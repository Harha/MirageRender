#ifndef INTERSECTION_H
#define INTERSECTION_H

// mirage includes
#include "../math/vec3.h"
#include "material.h"

namespace mirage
{

class Intersection
{
public:
    Intersection(vec3 position = vec3(), vec3 normal = vec3(), Material *material = nullptr);
    void setPosition(const vec3 &v);
    void setNormal(const vec3 &v);
    void setMaterial(Material *m);
    vec3 getPosition() const;
    vec3 getNormal() const;
    Material *getMaterial() const;
private:
    vec3 m_position;
    vec3 m_normal;
    Material *m_material;
};

}

#endif // INTERSECTION_H
