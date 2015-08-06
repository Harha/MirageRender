#ifndef TRIANGLE_H
#define TRIANGLE_H

// std includes
#include <array>

// mirage includes
#include "../core/shape.h"
#include "../core/vertex.h"

namespace mirage
{

class Triangle : public Shape
{
public:
    Triangle(const Transform &o2w, const Transform &w2o, std::array<Vertex, 3> vertices = { });
    virtual void update() override;
    virtual AABB objectBound() const override;
    virtual bool intersect(const Ray &ray, float &tHit, Intersection &iSect) const;
    virtual bool intersectP(const Ray &ray) const override;
    virtual float getSurfaceArea() const override;
    void getBarycentric(const vec3 &p, const vec3 &e1, const vec3 &e2, float &u, float &v, float &w) const;
    vec3 getMinimum() const;
    vec3 getMaximum() const;
private:
    std::array<Vertex, 3> m_verticesInit;
    std::array<Vertex, 3> m_verticesTransformed;
};

}

#endif // TRIANGLE_H
