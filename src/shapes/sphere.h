#ifndef SPHERE_H
#define SPHERE_H

// mirage includes
#include "../core/shape.h"

namespace mirage
{

class Sphere : public Shape
{
public:
    Sphere(const Transform &o2w, const Transform &w2o, float r = 1.0f);
    virtual AABB shapeBound() const override;
    virtual bool intersect(const Ray &ray, float &tHit, Intersection &iSect) const;
    virtual bool intersectP(const Ray &ray) const override;
    virtual float getSurfaceArea() const override;
    float getRadius() const;
private:
    float m_radius;
protected:
};

}

#endif // SPHERE_H
