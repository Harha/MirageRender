// std includes
#include <iostream>

// mirage includes
#include "accelerator.h"
#include "../macros.h"

namespace mirage
{

Accelerator::Accelerator(std::vector<Shape *> shapes) : m_shapes(shapes)
{
    m_initialized = false;
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
    AABB result = m_shapes[0]->worldBound();

    for (size_t i = 0; i < m_shapes.size(); i++)
    {
        result = result.addBox(m_shapes[i]->worldBound());
    }

    return result;
}

bool Accelerator::intersect(const Ray &ray, Intersection &iSect)
{
    ERR("Purely virtual function Accelerator::intersect called!");
    return false;
}

bool Accelerator::intersectP(const Ray &ray)
{
    ERR("Purely virtual function Accelerator::intersectP called!");
    return false;
}

void Accelerator::init()
{
    ERR("Purely virtual function Accelerator::build called!");
}

}
