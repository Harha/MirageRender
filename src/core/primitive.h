#ifndef PRIMITIVE_H
#define PRIMITIVE_H

// std includes
#include <vector>

// mirage includes
#include "transform.h"
#include "shape.h"
#include "aabb.h"
#include "ray.h"
#include "intersection.h"

namespace mirage
{

class Primitive
{
public:
    Primitive(const Transform &o2w = Transform(), const Transform &w2o = Transform(), const std::vector<Shape *> shapes = std::vector<Shape *>());
    virtual void update() const = 0;
    virtual AABB objectBound() const;
    virtual AABB worldBound() const;
    virtual bool intersect(const Ray &ray, Intersection &iSect) const = 0;
    virtual bool intersectP(const Ray &ray) const = 0;
private:
protected:
    const Transform &m_objToWorld;
    const Transform &m_worldToObj;
    std::vector<Shape *> m_shapes;
};

}

#endif // PRIMITIVE_H
