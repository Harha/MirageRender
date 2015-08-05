// std includes
#include <iostream>

// mirage includes
#include "primitive.h"
#include "../macros.h"

namespace mirage
{

Primitive::Primitive(const Transform &o2w, const Transform &w2o, std::vector<Shape *> shapes) : m_objToWorld(o2w), m_worldToObj(w2o), m_shapes(shapes)
{
    LOG("a New primitive was constructed! Number of shapes: " << m_shapes.size());
}

void Primitive::update() const
{
    ERR("Purely virtual function Primitive::update called!");
}

AABB Primitive::objectBound() const
{
    AABB result = m_shapes[0]->objectBound();

    for (size_t i = 0; i < m_shapes.size(); i++)
    {
        result = result.addBox(m_shapes[i]->objectBound());
    }

    return result;
}

AABB Primitive::worldBound() const // thinking... of how to nicely implement a k-d tree structure..
{
    return objectBound() * m_objToWorld.getMatrix();
}

bool Primitive::intersect(const Ray &ray, Intersection &iSect) const
{
    ERR("Purely virtual function Primitive::intersect called!");
    return false;
}

bool Primitive::intersectP(const Ray &ray) const
{
    ERR("Purely virtual function Primitive::intersectP called!");
    return false;
}

}
