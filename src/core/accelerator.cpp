// std includes
#include <iostream>

// mirage includes
#include "accelerator.h"
#include "../macros.h"

namespace mirage
{

Accelerator::Accelerator(const Transform &o2w, const Transform &w2o, std::vector<Shape *> shapes) : m_objToWorld(o2w), m_worldToObj(w2o), m_shapes(shapes)
{
    LOG("a New accelerator was constructed! Number of shapes: " << m_shapes.size());
}

void Accelerator::update() const
{
    ERR("Purely virtual function Accelerator::update called!");
}

AABB Accelerator::objectBound() const
{
    AABB result = m_shapes[0]->objectBound();

    for (size_t i = 0; i < m_shapes.size(); i++)
    {
        result = result.addBox(m_shapes[i]->objectBound());
    }

    return result;
}

AABB Accelerator::worldBound() const
{
    return objectBound() * m_objToWorld.getMatrix();
}

bool Accelerator::intersect(const Ray &ray, Intersection &iSect)
{
    ERR("Purely virtual function Accelerator::intersect called!");
    return false;
}

bool Accelerator::intersectP(const Ray &ray) const
{
    ERR("Purely virtual function Accelerator::intersectP called!");
    return false;
}

void Accelerator::build()
{
    ERR("Purely virtual function Accelerator::build called!");
}

}
