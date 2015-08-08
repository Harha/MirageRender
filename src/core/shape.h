#ifndef SHAPE_H
#define SHAPE_H

// mirage includes
#include "transform.h"
#include "material.h"
#include "aabb.h"
#include "ray.h"
#include "intersection.h"

namespace mirage
{

class Shape
{
public:
    Shape(const Transform &o2w = Transform(), const Transform &w2o = Transform(), Material m = Material());
    virtual void update() = 0;
    virtual AABB objectBound() const = 0;
    virtual AABB worldBound() const = 0;
    virtual bool intersect(const Ray &ray, Intersection &iSect) const = 0;
    virtual bool intersectP(const Ray &ray) const = 0;
    virtual float getSurfaceArea() const = 0;
    virtual void setMaterial(const Material &m);
    virtual Material getMaterial() const;
protected:
    const Transform &m_objToWorld;
    const Transform &m_worldToObj;
    Material m_material;
private:
};

}

#endif // SHAPE_H
