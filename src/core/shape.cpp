// std includes
#include <iostream>

// mirage includes
#include "shape.h"
#include "../macros.h"
#include "../math/math.h"

namespace mirage
{

Shape::Shape(const Transform o2w, Material *m) : m_objToWorld(o2w), m_worldToObj(o2w.inverse()), m_material(m)
{

}

Shape::~Shape()
{

}

void Shape::update()
{
    ERR("Purely virtual function Shape::update called!");
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

bool Shape::intersect(const Ray &ray, Intersection &iSect) const
{
    ERR("Purely virtual function Shape::intersect called!");
    return false;
}

bool Shape::intersectP(const Ray &ray) const
{
    ERR("Purely virtual function Shape::intersectP called!");
    return false;
}

float Shape::getSurfaceArea() const
{
    ERR("Purely virtual function Shape::getSurfaceArea called!");
    return 0.0f;
}

void Shape::setMaterial(Material &m)
{
    m_material = &m;
}

Material *Shape::getMaterial() const
{
    return m_material;
}

}
