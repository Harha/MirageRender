#ifndef INTERSECTION_H
#define INTERSECTION_H

// mirage includes
#include "../math/vec3.h"
#include "shape.h"

namespace mirage
{

class Intersection
{
public:
    Intersection(float t = INFINITY, vec3 position = vec3(), vec3 normal = vec3(), Shape shape = Shape());
    void setT(const float t);
    void setPosition(const vec3 &v);
    void setNormal(const vec3 &v);
    void setShape(const Shape &s);
    float getT() const;
    vec3 getPosition() const;
    vec3 getNormal() const;
    Shape getShape() const;
private:
    float m_t;
    vec3 m_position;
    vec3 m_normal;
    const Shape m_shape;

};

}

#endif // INTERSECTION_H
