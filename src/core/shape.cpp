// std includes
#include <iostream>

// mirage includes
#include "shape.h"
#include "../macros.h"
#include "../math/math.h"

namespace mirage
{

Shape::Shape(const Transform &o2w, const Transform &w2o, Material m) : m_objToWorld(o2w), m_worldToObj(w2o), m_material(m)
{

}

void Shape::update()
{
    ERR("Unimplemented Shape::update function called!");
}

AABB Shape::objectBound() const
{
    ERR("Unimplemented Shape::shapeBound function called!");
    return AABB();
}

AABB Shape::worldBound() const
{
    return objectBound() * m_objToWorld.getMatrix();
}

bool Shape::intersect(const Ray &ray, float &tHit, Intersection &iSect) const
{
    ERR("Unimplemented Shape::intersect function called!");
    return false;
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

void Shape::setMaterial(const Material &m)
{
    m_material = m;
}

Material Shape::getMaterial() const
{
    return m_material;
}

}
