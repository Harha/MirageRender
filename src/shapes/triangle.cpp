// std includes
#include <iostream>

// mirage includes
#include "triangle.h"
#include "../macros.h"

namespace mirage
{

Triangle::Triangle(const Transform o2w, Material *m, std::array<Vertex, 3> vertices) : Shape(o2w, m), m_verticesInit(vertices)
{
    update();
}

void Triangle::update()
{
    if (m_objToWorld.reqStateUpdate())
    {
        for (size_t i = 0; i < 3; i++)
        {
            m_verticesTransformed[i] = m_verticesInit[i] * m_objToWorld.getMatrix();
        }
    }
}

AABB Triangle::objectBound() const
{
    return AABB(getMinimum(m_verticesInit), getMaximum(m_verticesInit));
}

AABB Triangle::worldBound() const
{
    return AABB(getMinimum(m_verticesTransformed), getMaximum(m_verticesTransformed));
}

bool Triangle::intersect(const Ray &ray, Intersection &iSect) const
{
    vec3 P, Q, T;
    float d, inv_d, u, v, t, b0, b1, b2;

    std::array<Vertex, 3> vertices = m_verticesTransformed;

    const vec3 edge_a = vertices[1].getPosition() - vertices[0].getPosition();
    const vec3 edge_b = vertices[2].getPosition() - vertices[0].getPosition();

    P = vec3::cross(ray.getDirection(), edge_b);
    d = vec3::dot(edge_a, P);

    if (!m_material->isRefractive())
    {
        if (d < EPSILON)
            return false;
    }
    else
    {
        if (d > -EPSILON && d < EPSILON)
            return false;
    }

    inv_d = 1.0f / d;
    T = ray.getOrigin() - vertices[0].getPosition();
    u = vec3::dot(T, P) * inv_d;

    if (u < 0.0f || u > 1.0f)
        return false;

    Q = vec3::cross(T, edge_a);
    v = vec3::dot(ray.getDirection(), Q) * inv_d;

    if (v < 0.0f || u + v > 1.0f)
        return false;

    t = vec3::dot(edge_b, Q) * inv_d;

    if (t < EPSILON)
        return false;

    vec3 hit = ray(t);
    getBarycentric(hit, edge_a, edge_b, b0, b1, b2);
    vec3 N1 = vertices[0].getNormal();
    vec3 N2 = vertices[1].getNormal();
    vec3 N3 = vertices[2].getNormal();
    vec3 N = N1 + b1 * (N2 - N1) + b2 * (N3 - N1);

    if (d < 0.0f)
    {
        N = N.negate();
    }

    iSect.setT(t);
    iSect.setPosition(hit);
    iSect.setNormal(N.normalize());
    iSect.setMaterial(m_material);

    return true;
}

bool Triangle::intersectP(const Ray &ray) const
{
    vec3 P, Q, T;
    float d, inv_d, u, v, t;

    std::array<Vertex, 3> vertices = m_verticesTransformed;

    const vec3 edge_a = vertices[1].getPosition() - vertices[0].getPosition();
    const vec3 edge_b = vertices[2].getPosition() - vertices[0].getPosition();

    P = vec3::cross(ray.getDirection(), edge_b);
    d = vec3::dot(edge_a, P);

    if (d < EPSILON)
        return false;

    inv_d = 1.0f / d;
    T = ray.getOrigin() - vertices[0].getPosition();
    u = vec3::dot(T, P) * inv_d;

    if (u < 0.0f || u > 1.0f)
        return false;

    Q = vec3::cross(T, edge_a);
    v = vec3::dot(ray.getDirection(), Q) * inv_d;

    if (v < 0.0f || u + v > 1.0f)
        return false;

    t = vec3::dot(edge_b, Q) * inv_d;

    if (t < EPSILON)
        return false;

    return true;
}

float Triangle::getSurfaceArea() const
{
    return 0.0f;
}

void Triangle::getBarycentric(const vec3 &p, const vec3 &e1, const vec3 &e2, float &b0, float &b1, float &b2) const
{
    // Find the point from first vertice to the requested point
    const vec3 w = p - m_verticesTransformed[0].getPosition();

    // Find the perpendicular vectors
    const vec3 vCrossW = vec3::cross(e2, w);
    const vec3 uCrossW = vec3::cross(e1, w);
    const vec3 uCrossV = vec3::cross(e1, e2);

    // Find the barycentric coordinates
    const float denom = uCrossV.length();
    const float r = vCrossW.length() / denom;
    const float t = uCrossW.length() / denom;

    // Insert found values to the output variables
    b0 = 1.0f - r - t;
    b1 = r;
    b2 = t;
}

vec3 Triangle::getMinimum(const std::array<Vertex, 3> &v) const
{
    vec3 result;

    result.x = std::min(v[0].getPosition().x, std::min(v[1].getPosition().x, v[2].getPosition().x));
    result.y = std::min(v[0].getPosition().y, std::min(v[1].getPosition().y, v[2].getPosition().y));
    result.z = std::min(v[0].getPosition().z, std::min(v[1].getPosition().z, v[2].getPosition().z));

    return result;
}

vec3 Triangle::getMaximum(const std::array<Vertex, 3> &v) const
{
    vec3 result;

    result.x = std::max(v[0].getPosition().x, std::max(v[1].getPosition().x, v[2].getPosition().x));
    result.y = std::max(v[0].getPosition().y, std::max(v[1].getPosition().y, v[2].getPosition().y));
    result.z = std::max(v[0].getPosition().z, std::max(v[1].getPosition().z, v[2].getPosition().z));

    return result;
}

}
