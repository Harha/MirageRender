// std includes
#include <iostream>

// mirage includes
#include "sphere.h"
#include "../macros.h"

namespace mirage
{

Sphere::Sphere(const Transform &o2w, const Transform &w2o, vec3 c, float r) : Shape(o2w, w2o), m_centerInit(c), m_radiusInit(r)
{
    update();
}

void Sphere::update()
{
    if (m_objToWorld.reqStateUpdate())
    {
        vec4 c_i = vec4(m_centerInit.x, m_centerInit.y, m_centerInit.z, 1.0f);
        vec4 c_t = m_objToWorld.getMatrix() * c_i;
        m_centerTransformed = vec3(c_t.x, c_t.y, c_t.z);
        m_radiusTransformed = m_objToWorld.getScale().x * m_radiusInit;
    }
}

AABB Sphere::shapeBound() const
{
    return AABB(vec3(-m_radiusTransformed, -m_radiusTransformed, -m_radiusTransformed), vec3(m_radiusTransformed, m_radiusTransformed, m_radiusTransformed));
}

bool Sphere::intersect(const Ray &ray, float &tHit, Intersection &iSect) const
{
    vec3 C, SP;
    float r, b, d, t;

    // Get sphere center in world space
    C = m_centerTransformed;

    // Transform ray origin to object space
    SP = C - ray.getOrigin();

    // Get transformed radius
    r = m_radiusTransformed;

    // Solve the quadratic equation for t
    b = vec3::dot(SP, ray.getDirection());
    d = b * b - vec3::dot(SP, SP) + r * r;

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
    iSect.setNormal((iSect.getPosition() - C) / r);

    return true;
}

bool Sphere::intersectP(const Ray &ray) const
{
    vec3 C, SP;
    float r, b, d, t;

    // Get sphere center in world space
    C = m_centerTransformed;

    // Transform ray origin to object space
    SP = C - ray.getOrigin();

    // Get transformed radius
    r = m_radiusTransformed;

    // Solve the quadratic equation for t
    b = vec3::dot(SP, ray.getDirection());
    d = b * b - vec3::dot(SP, SP) + r * r;

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
    return 4.0f * PI * m_radiusTransformed * m_radiusTransformed;
}

vec3 Sphere::getCenterInit() const
{
    return m_centerInit;
}

vec3 Sphere::getCenterTransformed() const
{
    return m_centerTransformed;
}

float Sphere::getRadiusInit() const
{
    return m_radiusInit;
}

float Sphere::getRadiusTransformed() const
{
    return m_radiusTransformed;
}

}
