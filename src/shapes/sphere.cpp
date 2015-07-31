// std includes
#include <iostream>

// mirage includes
#include "sphere.h"
#include "../macros.h"

namespace mirage
{

Sphere::Sphere(const Transform &o2w, const Transform &w2o, float r) : Shape(o2w, w2o), m_r(r)
{

}

AABB Sphere::shapeBound() const
{
    ERR("Unimplemented Sphere::shapeBound function called!");
    return AABB();
}

AABB Sphere::worldBound() const
{
    ERR("Unimplemented Sphere::worldBound function called!");
    return AABB();
}

bool Sphere::intersectP(const Ray &ray) const
{
    ERR("Unimplemented Sphere::intersectP function called!");
    return false;
}

float Sphere::getSurfaceArea() const
{
    ERR("Unimplemented Sphere::getSurfaceArea function called!");
    return 0.0f;
}

float Sphere::getRadius() const
{
    return m_r;
}

}
