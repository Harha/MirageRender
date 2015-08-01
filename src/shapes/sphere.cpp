// std includes
#include <iostream>

// mirage includes
#include "sphere.h"
#include "../macros.h"

namespace mirage
{

Sphere::Sphere(const Transform &o2w, const Transform &w2o, float r) : Shape(o2w, w2o), m_radius(r)
{

}

AABB Sphere::shapeBound() const
{
    return AABB(vec3(-m_radius, -m_radius, -m_radius), vec3(m_radius, m_radius, m_radius));
}

bool Sphere::intersect(const Ray &ray, float &tHit, Intersection &iSect) const
{
    vec3 SP;
    float b, d, t;

    // Transform ray origin to object space
    SP = m_objToWorld.getPosition() - ray.getOrigin();

    // Solve the quadratic equation for t
    b = vec3::dot(SP, ray.getDirection());
    d = b * b - vec3::dot(SP, SP) + m_radius * m_radius;

    if (d < 0.0f)
    {
        return false;
    }

    d = std::sqrt(d);
    t = (t = b - d) > EPSILON ? t : ((t = b + d) > EPSILON ? t : -1.0f);

    if (t == -1.0f)
    {
        return false;
    }

    // Set final surface intersection info
    tHit = t;
    iSect.setPosition(ray.getOrigin() + ray.getDirection() * t);
    iSect.setNormal((iSect.getPosition() - m_objToWorld.getPosition()) / m_radius);

    return true;
}

bool Sphere::intersectP(const Ray &ray) const
{
    vec3 SP;
    float b, d, t;

    // Transform ray origin to object space
    SP = m_objToWorld.getPosition() - ray.getOrigin();

    // Solve the quadratic equation for t
    b = vec3::dot(SP, ray.getDirection());
    d = b * b - vec3::dot(SP, SP) + m_radius * m_radius;

    if (d < 0.0f)
    {
        return false;
    }

    d = std::sqrt(d);
    t = (t = b - d) > EPSILON ? t : ((t = b + d) > EPSILON ? t : -1.0f);

    if (t == -1.0f)
    {
        return false;
    }

    return true;
}

float Sphere::getSurfaceArea() const
{
    return 4.0f * PI * m_radius * m_radius;
}

float Sphere::getRadius() const
{
    return m_radius;
}

}
