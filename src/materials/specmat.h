#ifndef SPEC_H
#define SPEC_H

// std includes

// mirage includes
#include "../core/material.h"

namespace mirage
{

class SpecularMaterial : public virtual Material
{
public:
    SpecularMaterial(vec3 kd = vec3(0.9f, 0.9f, 0.9f), vec3 ks = vec3(), vec3 ke = vec3());
    virtual ~SpecularMaterial() override;
    virtual void evalBRDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &brdf) const override;
    virtual void evalBTDF(const vec3 &p, const vec3 &n, const vec3 &Wi, const vec3 &Wo, float &btdf) const override;
    virtual void evalPDF(float &pdf) const override;
    virtual void evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt) override;
private:
};

}

#endif // SPEC_H
