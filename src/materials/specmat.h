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
    virtual void evalBSDF(const vec3 &P, const vec3 &N, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const override;
    virtual void evalBSDF_direct(const vec3 &P, const vec3 &N, const vec3 &We, const vec3 &Wr, const vec3 &Wt, const vec3 &Wo, float &brdf, float &btdf) const override;
    virtual void evalPDF(float &pdf) const override;
    virtual void evalWi(const vec3 &Wo, const vec3 &N, vec3 &Wr, vec3 &Wt) override;
private:
};

}

#endif // SPEC_H
