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
    virtual AABB worldBound() const override;
    virtual bool intersectP(const Ray &ray) const override;
    virtual float getSurfaceArea() const override;
    float getRadius() const;
private:
    float m_r;
protected:
};

}

#endif // SPHERE_H
