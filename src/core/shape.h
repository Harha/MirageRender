#ifndef SHAPE_H
#define SHAPE_H

// mirage includes
#include "transform.h"
#include "aabb.h"
#include "ray.h"

namespace mirage
{

class Shape
{
public:
    Shape(const Transform &o2w, const Transform &w2o);
    virtual AABB shapeBound() const;
    virtual AABB worldBound() const;
    virtual bool intersectP(const Ray &ray) const;
    virtual float getSurfaceArea() const;
protected:
    const Transform &m_objToWorld;
    const Transform &m_worldToObj;
private:
};

}

#endif // SHAPE_H
