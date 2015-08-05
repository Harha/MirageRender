#ifndef ACCELERATOR_H
#define ACCELERATOR_H

// mirage includes
#include "primitive.h"

namespace mirage
{

class Accelerator
{
public:
    Accelerator(const Transform &o2w = Transform(), const Transform &w2o = Transform(), const std::vector<Shape *> shapes = std::vector<Shape *>());
    virtual void update() const = 0;
    virtual AABB objectBound() const;
    virtual AABB worldBound() const;
    virtual bool intersect(const Ray &ray, Intersection &iSect) = 0;
    virtual bool intersectP(const Ray &ray) const = 0;
    virtual void build() = 0;
private:
protected:
    const Transform &m_objToWorld;
    const Transform &m_worldToObj;
    std::vector<Shape *> m_shapes;
};

}

#endif // ACCELERATOR_H
