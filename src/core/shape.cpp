// std includes
#include <iostream>

// mirage includes
#include "shape.h"
#include "../macros.h"

namespace mirage
{

Shape::Shape(const Transform &o2w, const Transform &w2o) : m_objToWorld(o2w), m_worldToObj(w2o)
{

}

AABB Shape::shapeBound() const
{
    ERR("Unimplemented Shape::shapeBound function called!");
    return AABB();
}

AABB Shape::worldBound() const
{
    ERR("Unimplemented Shape::worldBound function called!");
    return AABB();
}

bool Shape::intersectP(const Ray &ray) const
{
    ERR("Unimplemented Shape::intersectP function called!");
    return false;
}

float Shape::getSurfaceArea() const
{
    ERR("Unimplemented Shape::getSurfaceArea function called!");
    return 0.0f;
}

}
