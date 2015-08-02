#ifndef SHAPE_H
#define SHAPE_H

// mirage includes
#include "transform.h"
#include "aabb.h"
#include "ray.h"
#include "intersection.h"

namespace mirage
{

class Shape
{
public:
    Shape(const Transform &o2w = Transform(), const Transform &w2o = Transform());
    virtual void update() = 0;
    virtual AABB shapeBound() const = 0;
    virtual AABB worldBound() const;
    virtual bool intersect(const Ray &ray, float &tHit, Intersection &iSect) const = 0;
    virtual bool intersectP(const Ray &ray) const = 0;
    virtual float getSurfaceArea() const = 0;
protected:
    const Transform &m_objToWorld;
    const Transform &m_worldToObj;
private:
};

}

#endif // SHAPE_H
